#ifndef AOC_CPP_UTILS_H
#define AOC_CPP_UTILS_H

#include <array>
#include <cctype>
#include <concepts>
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

template <typename T>
inline constexpr bool alwaysFalse(const T&) noexcept
{
    return false;
}

template <std::predicate<char> StopCharFunc = decltype(&alwaysFalse<char>)>
class SplitView
{
public:
    class iterator
    {
    public:
        using difference_type = std::ptrdiff_t;

        constexpr iterator(const SplitView& owner, const bool end = false) : 
            current_{end ? owner.str_.end() : owner.str_.begin()}, owner_{&owner}
        {
            update();
        }

        std::string_view& operator*() noexcept
        {
            return this->value_;
        }

        const std::string_view& operator*() const noexcept
        {
            return this->value_;
        }

        iterator& operator++()
        {
            update();
            return *this;
        }

        [[nodiscard]]
        iterator operator++(const int)
        {
            iterator copy{*this};
            update();
            return copy;
        }

        [[nodiscard]]
        constexpr bool operator==(const iterator& rhs)
        {
            return this->current_ == rhs.current_ && this->value_.empty() == rhs.value_.empty();
        }
    private:
        void update()
        {
            this->value_ = {};

            std::string_view::iterator stop{this->current_};

            while (stop != this->owner_->str_.end())
            {
                if (this->owner_->isStopChar_(*stop) || std::isspace(*stop))
                {
                    if (stop != this->current_)
                    {
                        this->value_ = std::string_view(this->current_, stop);
                        this->current_ = stop;
                        break;
                    }
                    else if (!std::isspace(*stop))
                    {
                        this->value_ = std::string_view{this->current_, this->current_ + 1};
                        ++this->current_;
                        stop = this->current_;
                        break;
                    }
                    else
                    {
                        ++stop;
                        this->current_ = stop;
                    }
                }
                else
                {
                    ++stop;
                }
            }

            if (stop != this->current_)
            {
                this->value_ = std::string_view(this->current_, stop);
                this->current_ = stop;
            }
        }

        std::string_view value_;
        std::string_view::iterator current_;
        const SplitView<StopCharFunc>* owner_{};
    };

    SplitView(const std::string_view str) :
        str_{str}
    {}

    template <typename T>
    SplitView(const std::string_view str, T&& isStopChar) :
        str_{str}, isStopChar_{std::forward<T>(isStopChar)}
    {}

    [[nodiscard]]
    constexpr iterator begin()
    {
        return {*this};
    }

    [[nodiscard]]
    constexpr iterator end()
    {
        return {*this, true};
    }
private:
    std::string_view str_;
    StopCharFunc isStopChar_{alwaysFalse<char>};
};

template <typename T>
SplitView(const std::string_view str, T&& isStopChar)
    -> SplitView<T>;

#endif