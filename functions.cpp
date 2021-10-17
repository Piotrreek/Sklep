#include <iostream>
#include <cstdio>
#include <thread>
#include <chrono>
#include <Windows.h>
#include "functions.h"
#include <string.h>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <list>
#include <iterator>
int start()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int choice = 0;
    setlocale(LC_ALL, "polish");
    SetConsoleTextAttribute(hConsole, 11);
    std::cout << "Witaj u¿ytkowniku w sklepie internetowym!\n" << std::endl;
    while (true)
    {
        std::cout << "Co chcesz zrobiæ ?\n\n\tWciœnij 1 aby przejœæ kupna przedmiotów.\n\n\tWciœnij 2 aby przejœæ do sprzeda¿y przedmiotów.\n\n\tWciœnij 3, aby wyœwietliæ iloœæ twojego towaru.\n\n\tWciœnij 4, aby zakoñczyæ dzia³anie programu.\n";
        while (!(std::cin >> choice))
        {
            std::cout << "To nie jest poprawny znak, podaj znak jeszcze raz: ";
            std::cin.clear();
            std::cin.ignore(300,'\n');
        }
        switch (choice)
        {
        case 1:
        {
            buy();
            system("cls");
            break;
        }
        case 2:
        {
            sell();
            system("cls");
            break;
        }
        case 3:
        {
            show();
            system("cls");
            break;
        }
        case 4:
        {
            system("cls");
            std::cout << "Program zostanie zamkniêty za 2 sekundy.";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            return 0;
            break;
        }
        default:
        {
            std::cout << "Wprowadzony znak nie jest poprawny\nWprowadŸ znak ponownie" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            system("cls");
            break;
        }
        }
    }
}
void buy() 
{
    struct buy_shop
    {
        std::string name;
        int price;
        int number;
    };
    struct buy_client
    {
        std::string name;
        int number;
    };

    int client_money;

    std::string product_to_buy;
    std::cout << "Jakis produkt chcesz kupic ?: __________ \b\b\b\b\b\b\b\b\b\b\b";
    std::cin >> product_to_buy;

    std::fstream client;
    client.open("client_items.txt", std::ios::in);

    std::list<buy_client> client_list; // lista struktur
    std::string line;

    getline(client, line); // pobieram 1 linijke z pieniedzmi
    std::stringstream ss;
    ss << line;
    ss >> client_money;

    buy_client file_client_line;
    while (getline(client, line))
    {
        std::stringstream ss;
        ss << line;
        ss >> file_client_line.name;
        ss >> file_client_line.number;
        client_list.push_back(file_client_line);
    }

    std::fstream shop;
    shop.open("shop_items.txt", std::ios::in);
    buy_shop file_shop_line;
    std::list<buy_shop> shop_list; // lista struktur
    while (getline(shop, line))
    {
        std::stringstream ss;
        ss << line;
        ss >> file_shop_line.name;
        ss >> file_shop_line.price;
        ss >> file_shop_line.number;
        shop_list.push_back(file_shop_line);
    }
    bool find_client = false;
    bool find_shop = false;

    std::list<buy_client>::iterator i1 = client_list.begin();   // szukamy przedmiotu ktory nas interesuje
    for (buy_client x : client_list)
    {
        if (x.name == product_to_buy)
        {
            find_client = true;
            break;
        }
        i1++;
    }
    std::list<buy_shop>::iterator i2 = shop_list.begin();
    for (buy_shop x : shop_list)
    {
        if (x.name == product_to_buy)
        {
            find_shop = true;
            break;
        }
        i2++;
    }

    if (find_client == true && find_shop == true)
    {
        if ((client_money - i2->price >= 0) && (i2->number > 0))
        {
            i1->number++;
            client_money -= i2->price;
            i2->number--;
        }
        else if ((client_money - i2->price) < 0)
        {
            std::cout << "Nie stac cie na ten przedmiot\n";
            Sleep(1000);
        }
        else
        {
            std::cout << "Nie mamy aktualnie tego przedmiotu\n";
            Sleep(1000);
        }
    }
    else if (find_client == false && find_shop == true && i2->number > 0 && (client_money - i2->price >= 0))
    {
        buy_client item;
        item.name = product_to_buy;
        item.number = 1;
        i2->number--;
        client_money -= i2->price;
        client_list.push_back(item);
    }
    else if (find_shop == false)
    {
        std::cout << "Nie mamy takiego przedmiotu\n";
        Sleep(1000);
    }

    shop.close();
    client.close();

    shop.open("shop_items.txt", std::ios::out);
    client.open("client_items.txt", std::ios::out);

    for (buy_shop x : shop_list)
    {
        shop << x.name + " " << std::to_string(x.price) + " " << std::to_string(x.number) + "\n";
    }
    client << std::to_string(client_money);
    for (buy_client x : client_list)
    {
        client << "\n" + x.name + " " << std::to_string(x.number);
    }
}
void show()
{
    system("cls");
    std::fstream odczytaj_dane;
    odczytaj_dane.open("client_items.txt", std::ios::in); // otwieramy plik do odczytu danych
    std::string odczytaj;
    if (odczytaj_dane.good()) // sprawdzamy czy plik sie otworzyl
    {
        getline(odczytaj_dane, odczytaj); // pobieramy pierwsza linijke, w ktorej jest ilosc pieniedzy
            while (getline(odczytaj_dane, odczytaj))
            {
                std::stringstream wypisz_na_ekran;
                wypisz_na_ekran << odczytaj;
                std::string nazwa_przedmiotu = "";
                int ilosc_przedmiotu;
                wypisz_na_ekran >> nazwa_przedmiotu >> ilosc_przedmiotu;
                if (nazwa_przedmiotu != "")
                    std::cout << "Posiadasz " << ilosc_przedmiotu << " " << nazwa_przedmiotu << std::endl;
            }
            std::cout << "\nAby kontynuowac, nacisnij enter";
            std::cin.get();
            std::cin.get();
    }
}
void sell()
{
    struct product_to_sell_client
    {
        std::string product_name;
        int product_number_client;
    };
    struct product_to_sell_shop
    {
        std::string product_name;
        int product_number_shop;
        int price;
    };
    int client_money;
    std::string product_to_sell;
    std::cout << "Jaki produkt chcesz sprzedaæ ?:  __________ \b\b\b\b\b\b\b\b\b\b\b";
    std::cin >> product_to_sell;

    std::fstream client;
    client.open("client_items.txt", std::ios::in);

    std::list<product_to_sell_client> client_list; // lista struktur
    std::string line;

    getline(client, line); // pobieram 1 linijke z pieniedzmi
    std::stringstream ss;
    ss << line;
    ss >> client_money;

    product_to_sell_client file_client_line;
    while (getline(client, line))
    {
        std::stringstream ss;
        ss << line;
        ss >> file_client_line.product_name;
        ss >> file_client_line.product_number_client;
        client_list.push_back(file_client_line);
    }
    
    std::fstream shop;
    shop.open("shop_items.txt", std::ios::in);
    product_to_sell_shop file_shop_line;
    std::list<product_to_sell_shop> shop_list; // lista struktur
    while (getline(shop, line))
    {
        std::stringstream ss;
        ss << line;
        ss >> file_shop_line.product_name;
        ss >> file_shop_line.price;
        ss >> file_shop_line.product_number_shop;
        shop_list.push_back(file_shop_line);
    }

    std::list<product_to_sell_client>::iterator i1 = client_list.begin();   // szukamy przedmiotu ktory nas interesuje
    for (product_to_sell_client x : client_list)
    {
        if (x.product_name == product_to_sell)
            break;
        i1++;
    }
    std::list<product_to_sell_shop>::iterator i2 = shop_list.begin();
    for (product_to_sell_shop x : shop_list)
    {
        if (x.product_name == product_to_sell)
            break;
        i2++;
    }
    
    if (i1->product_name != i2->product_name)
    {
        if (i1->product_name != product_to_sell)
        {
            std::cout << "Nie masz takiego produktu\n";
            Sleep(1000);
        }
        else
        {
            std::cout << "Sklep nie przyjmuje takiego produktu\n";
            Sleep(1000);
        }
    }
    else
    {
        i1->product_number_client--;
        client_money += int(i2->price * 0.9);
        i2->product_number_shop++;
    }

    shop.close();
    client.close();

    shop.open("shop_items.txt", std::ios::out);
    client.open("client_items.txt", std::ios::out);

    for (product_to_sell_shop x : shop_list)
    {
        shop << x.product_name +" " << std::to_string(x.price) +" " << std::to_string(x.product_number_shop) + "\n";
    }
    client << std::to_string(client_money) + "\n";
    for (product_to_sell_client x : client_list)
    {
        client << x.product_name + " " << std::to_string(x.product_number_client) + "\n";
    }
}

