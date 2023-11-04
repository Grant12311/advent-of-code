#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

static constexpr std::size_t BITS_PER_HEX_DIGIT{4};
static constexpr std::size_t BITS_PER_BYTE{8};
static constexpr std::size_t BITS_PER_CHILD_COUNT{11};
static constexpr std::size_t BITS_PER_CHILD_LENGTH{15};

/**
 * @brief Read input hex data from file
 * 
 * @return Contents of first (only) line as a string
 */
[[nodiscard]]
static std::string readInput()
{
    std::ifstream infile{"input.txt"};
    if (!infile.is_open())
        throw std::runtime_error{"Cannot open input file"};

    std::string input;
    std::getline(infile, input);

    return input;
}

/**
 * @brief Convert hex digit to decimal
 * 
 * @param digit Hex digit to convert
 * @return Hex digit in decimal form
 */
[[nodiscard]]
static unsigned char hexDigitToU8(const char digit)
{
    return static_cast<unsigned char>(digit <= '9' ? digit - '0' : digit - 'A' + static_cast<char>(10));
}

/**
 * @brief Convert hex string into binary 
 * 
 * @param hex Hex string to convert
 * @return Binary represention of string
 */
[[nodiscard]]
static std::vector<bool> hexToBinary(const std::string& hex)
{
    std::vector<bool> result;
    result.reserve(hex.size() * BITS_PER_HEX_DIGIT);

    for (const char digit : hex)
    {
        const unsigned char byte{hexDigitToU8(digit)};

        for (int bit{BITS_PER_HEX_DIGIT - 1}; bit > -1; --bit)
        {
            result.push_back((byte & (1 << bit)) >> bit);
        }
    }

    return result;
}

struct Packet
{
    unsigned char version{};

    enum class Type
    {
        LITERAL = 4
    };
    Type type{};

    // Only one will be used per packet
    unsigned long long int value;
    std::vector<Packet> children;
};

/**
 * @brief Convert "bits" (at most 8) bits of binary data starting at "it" into decimal
 * 
 * @param it Starting iterator
 * @param bits Number of bits to convert. At most 8
 * @return Decimal representation of binary data
 */
static unsigned char bitsToU8(std::vector<bool>::const_iterator it, const int bits)
{
    assert(static_cast<std::size_t>(bits) <= sizeof(unsigned short int) * BITS_PER_BYTE);

    unsigned char value{};

    for (int i{bits - 1}; i > -1; --i, ++it)
    {
        value |= static_cast<unsigned char>((*it ? 1 : 0) << i);
    }

    return value;
}

/**
 * @brief Convert "bits" (at most 16) bits of binary data starting at "it" into decimal
 * 
 * @param it Starting iterator
 * @param bits Number of bits to convert. At most 16
 * @return Decimal representation of binary data
 */
static unsigned short int bitsToU16(std::vector<bool>::const_iterator it, const int bits)
{
    assert(static_cast<std::size_t>(bits) <= sizeof(unsigned short int) * BITS_PER_BYTE);

    unsigned short int value{};

    for (int i{bits - 1}; i > -1; --i, ++it)
    {
        value |= static_cast<unsigned char>((*it ? 1 : 0) << i);
    }

    return value;
}

static Packet parsePacket(const std::vector<bool>& input, std::size_t& i);

/**
 * @brief Parse literal for packet value from input
 * 
 * @param input Binary packet data
 * @param i Index to start parsing
 * @return Decimal representation of literal value fo packet
 */
static unsigned long long int parseLiteralValue(const std::vector<bool>& input, std::size_t& i)
{
    std::vector<unsigned char> segments;

    // Run loop until a 4-bit chunk is prefixed by a zero

    bool last{!input[i]};
    do
    {
        last = !input[i];
        ++i;

        // Read one segment (hex digit) worth of binary
        segments.push_back(bitsToU8(input.begin() + static_cast<std::ptrdiff_t>(i), 4));
        i += BITS_PER_HEX_DIGIT;
    } while (!last);

    if (segments.size() > sizeof(unsigned long long int) * 2)
        throw std::runtime_error{std::string{1, '"'} + std::to_string(segments.size()) + "\" is too many segemtns for literal"};

    // Concatinate segments
    unsigned long long int value{};
    for (std::size_t j{}; j < segments.size(); ++j)
    {
        value |= static_cast<unsigned long long int>(segments[j]) << ((segments.size() - j- 1) * BITS_PER_HEX_DIGIT);
    }

    return value;
}

/**
 * @brief Parse children of a packet that uses a child count
 * 
 * @param input Binary packet data
 * @param i Index to start parsing
 * @return Array of all child packets
 */
static std::vector<Packet> parseChildrenByCount(const std::vector<bool>& input, std::size_t& i)
{
    std::vector<Packet> children;

    const std::size_t count{bitsToU16(input.begin() + static_cast<std::ptrdiff_t>(i), BITS_PER_CHILD_COUNT)};
    i += BITS_PER_CHILD_COUNT;

    children.reserve(count);

    for (std::size_t j{}; j < count; ++j)
    {
        children.push_back(parsePacket(input, i));
    }

    return children;
}

/**
 * @brief Parse children of a packet that uses a child length
 * 
 * @param input Binary packet data
 * @param i Index to start parsing
 * @return Array of all child packets
 */
static std::vector<Packet> parseChildrenByLength(const std::vector<bool>& input, std::size_t& i)
{
    std::vector<Packet> children;

    const std::size_t length{bitsToU16(input.begin() + static_cast<std::ptrdiff_t>(i), BITS_PER_CHILD_LENGTH)};
    i += BITS_PER_CHILD_LENGTH;

    const std::size_t end{i + length};

    while (i < end)
    {
        children.push_back(parsePacket(input, i));
    }

    return children;
}

/**
 * @brief Parse all children of an operator packet
 * 
 * @param input Binary packet data
 * @param i Index to start parsing
 * @return Array of all child packets
 */
static std::vector<Packet> parseChildren(const std::vector<bool>& input, std::size_t& i)
{
    const bool countMode{input[i++]};

    if (countMode)
        return parseChildrenByCount(input, i);
    else
        return parseChildrenByLength(input, i);
}

/**
 * @brief Parse a packet and all of it's children, recusively
 * 
 * @param input Binary packet data
 * @param i Index to start parsing
 * @return Packet 
 */
static Packet parsePacket(const std::vector<bool>& input, std::size_t& i)
{
    Packet packet;

    packet.version = bitsToU8(input.begin() + static_cast<std::ptrdiff_t>(i), 3);
    i += 3;

    packet.type = static_cast<Packet::Type>(bitsToU8(input.begin() + static_cast<std::ptrdiff_t>(i), 3));
    i += 3;

    switch (packet.type)
    {
        case Packet::Type::LITERAL:
            packet.value = parseLiteralValue(input, i);
            break;
        default:
            packet.children = parseChildren(input, i);
            break;
    }

    return packet;
}

static int sumVersions(const Packet& packet)
{
    int sum{packet.version};

    for (const Packet& child : packet.children)
    {
        sum += sumVersions(child);
    }

    return sum;
}

/**
 * @brief 
 * 
 * @param input Binary packet data
 * @return Packet 
 */
static Packet parsePacket(const std::vector<bool>& input)
{
    std::size_t i{};
    return parsePacket(input, i);
}

int main()
{
    const std::vector<bool> input{hexToBinary(readInput())};
    const Packet root{parsePacket(input)};

    std::cout << sumVersions(root) << '\n';

    return 0;
}