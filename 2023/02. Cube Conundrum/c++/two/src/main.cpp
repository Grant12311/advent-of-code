#include <algorithm>
#include <array>
#include <cctype>
#include <concepts>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// Represents a token of input text
struct Token
{
    enum class Type
    {
        NONE,

        COLON,
        SEMICOLON,
        COMMA,

        GAME,
        RED,
        GREEN,
        BLUE,

        NUMBER
    };
    Type type{};

    // Numeric value of NUMBER tokens
    int value{};

    // Contstruct a Token by parsing a string
    Token(const std::string_view text)
    {
        if (text == ":")
        {
            this->type = Type::COLON;
        }
        else if (text == ";")
        {
            this->type = Type::SEMICOLON;
        }
        else if (text == ",")
        {
            this->type = Type::COMMA;
        }
        else if (text == "Game")
        {
            this->type = Type::GAME;
        }
        else if (text == "red")
        {
            this->type = Type::RED;
        }
        else if (text == "green")
        {
            this->type = Type::GREEN;
        }
        else if (text == "blue")
        {
            this->type = Type::BLUE;
        }
        else
        {
            // Assume token is a number of nothing else matches
            // std::invalid_argument will be thrown if text is not a number
            this->type = Type::NUMBER;
            this->value = std::stoi(text.data());
        }
    }
};

// Characters that mark the end of a multi-char token
static constexpr std::array<char, 3> STOP_CHARS{
    ':',
    ';',
    ','
};

/**
 * @brief Check if a char is either whitespace or a stop character
 * 
 * @param c 
 * @return true 
 * @return false 
 */
static bool isStopChar(const char c)
{
    return std::isspace(c) || std::ranges::find(STOP_CHARS, c) != STOP_CHARS.end();
}

/**
 * @brief Split a line into an array of Tokens
 * 
 * @param line 
 * @return std::vector<Token> 
 */
[[nodiscard]]
static std::vector<Token> tokenize(const std::string& line)
{
    std::vector<Token> tokens;

    // Start of current multi-char token
    std::string::const_iterator start{line.begin()};

    for (std::string::const_iterator it{line.begin()}; it != line.end(); ++it)
    {
        if (isStopChar(*it))
        {
            // If a multi-char token was being recorded, it's over now and should be hanlded
            if (start != it)
            {
                // Remove any leading whitespace
                while (std::isspace(*start))
                {
                    ++start;
                }

                // Construct string view from iterators and hand it to Token constructor for parsing
                tokens.emplace_back(std::string_view{start, it});
            }
            // Reset multi-char token iterator to current position
            start = it;
        }
    }

    // Same steps as above to handle flush of end multi-char toke
    if (start != line.end())
    {
        while (std::isspace(*start))
        {
            ++start;
        }

        tokens.emplace_back(std::string_view{start, line.end()});
    }

    return tokens;
}

// Throws std::runtime_error if token does not belong to any of the Token::Type values passed as args
template <typename... Args>
    requires (std::same_as<Args, Token::Type>, ...)
void expect(const Token token, Args&&... args)
{
    if (((token.type != args) && ...))
        throw std::runtime_error{"Unexpected token"};
}

static constexpr std::size_t NUM_COLORS{3};

int main(const int argc, const char** const argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <INPUT_FILE>\n";
        return 1;
    }

    std::ifstream infile{argv[1]};
    if (!infile.is_open())
    {
        std::cerr << "ERROR! Failed to open " << std::quoted(argv[1]) << '\n';
        return 1;
    }

    // Puzzel answer
    int powerSum{};

    int lineNum = 1;
    for (std::string line; std::getline(infile, line); ++lineNum)
    {
        // Minimum number of cubes of each color needed for game to be possible
        std::array<int, NUM_COLORS> minNeeded{};

        const std::vector<Token> tokens{tokenize(line)};

        // Sanity check line opening
        expect(tokens[0], Token::Type::GAME);
        expect(tokens[1], Token::Type::NUMBER);
        expect(tokens[2], Token::Type::COLON);

        // Read all (ammount, color) pairs
        // The difference between commas and semicolons doesn't actually matter and is ignored
        for (std::size_t i{3}; i < tokens.size(); i += 3)
        {
            expect(tokens[i], Token::Type::NUMBER);
            const int ammount{tokens[i].value};

            // Parse color. Subtracting RED from token type converts color to 0-2 range for array
            expect(tokens[i + 1], Token::Type::RED, Token::Type::GREEN, Token::Type::BLUE);
            const std::size_t color{static_cast<std::size_t>(tokens[i + 1].type) - static_cast<std::size_t>(Token::Type::RED)};

            // Check if ammount changes the min for this color
            minNeeded[color] = std::max(minNeeded[color], ammount);
        }

        // Calculate "power" of minimums and add it to sum
        powerSum += minNeeded[0] * minNeeded[1] * minNeeded[2];
    }

    std::cout << "Result: " << powerSum << '\n';

    return 0;
}