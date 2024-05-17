#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
public:
    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
    Domain(string name) :
        name_("."s + name) {
    }

    // разработайте operator==
    bool operator==(const Domain &other) const {
        return this == &other || name_ == other.name_;
    }

    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    [[nodiscard]] bool IsSubdomain(const Domain &other) const {
        return *this == other ||
               (name_.size() > other.name_.size() &&
                std::mismatch(
                        other.name_.rbegin(),
                        other.name_.rend(),
                        name_.rbegin()
                        ).first == other.name_.rend());
    }

    operator string_view() const {
        return name_;
    }

private:
    string name_;

};

class DomainChecker {
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template<typename It>
    DomainChecker(It begin, It end) :
        forbidden_domains_(begin, end) {
        std::sort(forbidden_domains_.begin(), forbidden_domains_.end(),
                  [](const string_view &lhs, const string_view &rhs) {
                      return lexicographical_compare(lhs.rbegin(), lhs.rend(), rhs.rbegin(), rhs.rend());
                  });
        forbidden_domains_.erase(
                std::unique(forbidden_domains_.begin(), forbidden_domains_.end(),
                            [](const Domain &lhs, const Domain &rhs) {
                                return lhs.IsSubdomain(rhs) || rhs.IsSubdomain(lhs);
                            }),
                forbidden_domains_.end());
    }

    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    [[nodiscard]] bool IsForbidden(const Domain &domain) const {
        if (forbidden_domains_.empty()) {
            return false;
        }
        auto it = std::upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain,
                                   [](const string_view &val, const string_view &cur) {
                                       return lexicographical_compare(val.rbegin(), val.rend(), cur.rbegin(),
                                                                      cur.rend());
                                   });
        if (it == forbidden_domains_.begin()) {
            return false;
        }
        return domain.IsSubdomain(*prev(it));
    }

private:
    std::vector<Domain> forbidden_domains_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
vector<Domain> ReadDomains(istream &input, size_t count) {
    vector<Domain> domains;
    domains.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        string domain;
        input >> domain;
        domains.emplace_back(domain);
    }

    return domains;
}

template<typename Number>
Number ReadNumberOnLine(istream &input) {
    Number num;
    input >> num;
    return num;
}

void ProcessQueries(istream &input, ostream &output) {
    const std::vector<Domain> forbidden_domains = ReadDomains(input, ReadNumberOnLine<size_t>(input));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(input, ReadNumberOnLine<size_t>(input));
    for (const Domain &domain: test_domains) {
        output << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}

void TestDomain() {
    {
        Domain domain("ya.ru");
        assert(domain == Domain("ya.ru"));
        assert(domain != Domain("ya.ru."));
    }

    {
        Domain domain("ya.ru");
        assert(domain.IsSubdomain(Domain("ru")));
        assert(domain.IsSubdomain(Domain("ya.ru")));
        assert(!domain.IsSubdomain(Domain("m.ya.ru")));
        assert(Domain("m.ya.ru").IsSubdomain(domain));
    }
}

void TestDomainChecker() {
    {
        // Empty forbidden domains
        std::vector<string> domains;
        DomainChecker checker(domains.begin(), domains.end());
        assert(!checker.IsForbidden("ya.ru"s));
    }
    {
        // Test single domain
        std::vector<string> domains {"a"};
        DomainChecker checker(domains.begin(), domains.end());
        assert(checker.IsForbidden("a"s));
        assert(checker.IsForbidden("b.a"s));
        assert(!checker.IsForbidden("b"s));
    }
    {
        // Test subdomains
        std::vector<string> domains {
                "gdz.ru",
                "maps.me",
                "m.gdz.ru",
                "com"
        };
        DomainChecker checker(domains.begin(), domains.end());
        assert(checker.IsForbidden("gdz.ru"s));
        assert(checker.IsForbidden("m.gdz.ru"s));
        assert(checker.IsForbidden("gdz.com"s));
        assert(!checker.IsForbidden("gdz.com.ru"s));
        assert(checker.IsForbidden("m.maps.me"s));
        assert(checker.IsForbidden("maps.com"s));
        assert(!checker.IsForbidden("maps.ru"s));
        assert(!checker.IsForbidden("gdz.ua"s));
    }
}

void RunTests() {
    TestDomain();
    TestDomainChecker();
}

int main() {
    RunTests();
    ProcessQueries(cin, cout);
}
