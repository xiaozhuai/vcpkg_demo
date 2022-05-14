/**
 * Copyright 2022 xiaozhuai
 */

#include <cstdio>
#include <optional>
#include <vector>

#include "nlohmann/json.hpp"

namespace test {

struct Person {
    std::string name;
    int age = 0;
    std::vector<std::string> hobbies;
    std::optional<std::string> address;
};

struct Employee {
    Person person;
    std::string company;
    std::string title;
    std::optional<std::string> department;
};

}  // namespace test

namespace nlohmann {

template <>
struct adl_serializer<test::Person> {
    static void to_json(json &j, const test::Person &obj) {
        j["name"] = obj.name;
        j["age"] = obj.age;
        j["hobbies"] = obj.hobbies;
        if (obj.address.has_value()) {
            j["address"] = obj.address.value();
        }
    }

    static void from_json(const json &j, test::Person &obj) {
        obj.name = j.at("name");
        obj.age = j.at("age");
        obj.hobbies = j.at("hobbies");
        if (j.contains("address")) {
            obj.address = j.at("address");
        } else {
            obj.address = std::nullopt;
        }
    }
};

template <>
struct adl_serializer<test::Employee> {
    static void to_json(json &j, const test::Employee &obj) {
        j["person"] = obj.person;
        j["company"] = obj.company;
        j["title"] = obj.title;
        if (obj.department.has_value()) {
            j["department"] = obj.department.value();
        } else {
            j["department"] = nullptr;
        }
    }

    static void from_json(const json &j, test::Employee &obj) {
        obj.person = j.at("person");
        obj.company = j.at("company");
        obj.title = j.at("title");
        if (j.contains("department") && !j.at("department").is_null()) {
            obj.department = j.at("department");
        } else {
            obj.department = std::nullopt;
        }
    }
};

}  // namespace nlohmann

int main() {
    std::string json = R"(
{
    "person": {
        "name": "xiaozhuai",
        "age": 29,
        "hobbies": [
            "coding",
            "reading",
            "gaming"
        ],
        "address": "Shanghai, China"
    },
    "company": "wuta",
    "title": "engineer",
    "department": null
}
)";

    test::Employee employee = nlohmann::json::parse(json);
    printf("person: \n");
    printf("  - name: %s\n", employee.person.name.c_str());
    printf("  - age: %d\n", employee.person.age);
    printf("  - hobbies: \n");
    for (const auto &hobby : employee.person.hobbies) {
        printf("    - %s\n", hobby.c_str());
    }
    if (employee.person.address.has_value()) {
        printf("  - address: %s\n", employee.person.address.value().c_str());
    }
    printf("company: %s\n", employee.company.c_str());
    printf("title: %s\n", employee.title.c_str());
    if (employee.department.has_value()) {
        printf("department: %s\n", employee.department.value().c_str());
    }

    std::string json2 = nlohmann::json(employee).dump(4);
    printf("json: %s\n", json2.c_str());

    return 0;
}
