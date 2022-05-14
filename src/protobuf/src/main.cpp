/**
 * Copyright 2022 xiaozhuai
 */

#include <fstream>
#include <iostream>

#include "proto/address_book.pb.h"

void PromptForAddress(data::Person *person) {
    std::cout << "Enter person ID number: ";
    int id;
    std::cin >> id;
    person->set_id(id);
    std::cin.ignore(256, '\n');

    std::cout << "Enter name: ";
    getline(std::cin, *person->mutable_name());

    std::cout << "Enter email address (blank for none): ";
    std::string email;
    getline(std::cin, email);
    if (!email.empty()) {
        person->set_email(email);
    }

    while (true) {
        std::cout << "Enter a phone number (or leave blank to finish): ";
        std::string number;
        getline(std::cin, number);
        if (number.empty()) {
            break;
        }

        data::PhoneNumber *phone_number = person->add_phones();
        phone_number->set_number(number);

        std::cout << "Is this a mobile, home, or work phone? ";
        std::string type;
        getline(std::cin, type);
        if (type == "mobile") {
            phone_number->set_type(data::PhoneType::MOBILE);
        } else if (type == "home") {
            phone_number->set_type(data::PhoneType::HOME);
        } else if (type == "work") {
            phone_number->set_type(data::PhoneType::WORK);
        } else {
            std::cout << "Unknown phone type.  Using default." << std::endl;
        }
    }
}

void ListPeople(const data::AddressBook &address_book) {
    for (int i = 0; i < address_book.people_size(); i++) {
        const data::Person &person = address_book.people(i);

        std::cout << "Person ID: " << person.id() << std::endl;
        std::cout << "  Name: " << person.name() << std::endl;
        if (person.has_email()) {
            std::cout << "  E-mail address: " << person.email() << std::endl;
        }

        for (int j = 0; j < person.phones_size(); j++) {
            const data::PhoneNumber &phone_number = person.phones(j);

            switch (phone_number.type()) {
                case data::PhoneType::MOBILE:
                    std::cout << "  Mobile phone #: ";
                    break;
                case data::PhoneType::HOME:
                    std::cout << "  Home phone #: ";
                    break;
                case data::PhoneType::WORK:
                    std::cout << "  Work phone #: ";
                    break;
                default:
                    break;
            }
            std::cout << phone_number.number() << std::endl;
        }
    }
}

int main(int argc, char **argv) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    if (argc != 2) {
        std::cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << std::endl;
        return -1;
    }

    data::AddressBook address_book;

    {
        std::fstream input(argv[1], std::ios::in | std::ios::binary);
        if (!input) {
            std::cout << argv[1] << ": File not found.  Creating a new file." << std::endl;
        } else if (!address_book.ParseFromIstream(&input)) {
            std::cerr << "Failed to parse address book." << std::endl;
            return -1;
        }
    }

    ListPeople(address_book);

    PromptForAddress(address_book.add_people());

    {
        std::fstream output(argv[1], std::ios::out | std::ios::trunc | std::ios::binary);
        if (!address_book.SerializeToOstream(&output)) {
            std::cerr << "Failed to write address book." << std::endl;
            return -1;
        }
    }

    ListPeople(address_book);

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
