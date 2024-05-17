#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>


template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    os << "[";
    bool is_first = true;
    for (const auto& el: vec) {
        if (is_first) {
            os << el;
            is_first = false;
            continue;
        }
        os << ", " << el;
    }
    os << "]";
    return os;
}

class EBook {
public:
    void Read(size_t user_id, uint16_t page) {
        if (users_progress_.size() <= user_id) {
            users_progress_.resize(user_id + 1, 0);
        }
        if (users_progress_[user_id] == 0){
            ++users_amount_;
        } else {
            --users_page_[users_progress_[user_id]];
        }
        ++users_page_[page];
        users_progress_[user_id] = page;
    }

    [[nodiscard]] double Cheer(size_t user_id) const noexcept {
        if (users_amount_ == 0 || users_amount_ == 1) {
            return static_cast<double>(users_amount_);
        }
        if (user_id > users_progress_.size() || users_progress_[user_id] == 0) {
            return 0;
        }
        const uint16_t page = users_progress_[user_id];
        const size_t amount = std::accumulate(users_page_.begin(), users_page_.begin() + page, 0ull);
        return static_cast<double>(amount) / (users_amount_ - 1);
    }

private:
    size_t users_amount_ {0};
    std::vector<uint16_t> users_progress_ {};
    std::vector<size_t> users_page_ {std::vector<size_t>(1001, 0)};
};

struct Command {
    std::string type;
    size_t id;
    uint16_t page;
};

std::vector<Command> ReadCommands(std::istream &input) {
    using namespace std::literals;
    std::vector<Command> commands;

    size_t command_ammount;
    input >> command_ammount;

    commands.reserve(command_ammount);

    for (size_t i = 0; i < command_ammount; ++i) {
    Command cmd;
    input >> cmd.type;
    if (cmd.type == "READ"s) {
        input >> cmd.id >> cmd.page;
    } else if (cmd.type == "CHEER"s) {
        input >> cmd.id;
    } else {
        throw std::runtime_error("The \""s + cmd.type +
                                 "\" command does not exist"s);
    }
    commands.emplace_back(cmd);
    }

    return commands;
}

void ApplyCommands(EBook &ebook, const std::vector<Command> &commands,
                     std::ostream &out) {
    using namespace std::literals;
    for (const Command &cmd : commands) {
    if (cmd.type == "READ"s) {
        ebook.Read(cmd.id, cmd.page);
    } else if (cmd.type == "CHEER"s) {
        out.precision(6);
        out << ebook.Cheer(cmd.id) << std::endl;
    } else {
        throw std::runtime_error("The \""s + cmd.type +
                                 "\" command does not exist"s);
    }
    }
}

#include <fstream>

int main() {
    std::ifstream test_file("test.txt");
    EBook ebook;
    ApplyCommands(ebook, ReadCommands(std::cin), std::cout);
    return 0;
}