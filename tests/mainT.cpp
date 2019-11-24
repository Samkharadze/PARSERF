

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

    TEST(Json_nlohman, parse) 
	{
        json array1 = json::array();
        json my_json = json::array
                (
                {
                    {"Si-9.15",                      "RTS-9.15", "GAZP-9.15"},
                    {100024,                         100027,     100050},
                    {"Futures contract for USD/RUB", "Futures contract for index RTS",
                     "Futures contract for Gazprom shares"}
                    }
                );
        for (unsigned long int j = 0; j < my_json.size(); j++)
        {   int count = 0;
            json obj = json::object();
            for (unsigned long int i = 0; i < my_json.size(); i++, count++) {
                switch (count)
                {
                    case 0:
                        obj["ticker"] = my_json[i][j];
                        break;
                    case 1:
                        obj["id"] = my_json[i][j];
                        break;
                    case 2:
                        obj["description"] = my_json[i][j];
                        break;
                }
            }
            array1.push_back(obj);
        }
        my_json = array1;
        EXPECT_EQ(my_json[0]["ticker"], "Si-9.15");
        EXPECT_EQ(my_json[1]["ticker"], "RTS-9.15");
        EXPECT_EQ(my_json[2]["ticker"], "GAZP-9.15");
        EXPECT_EQ(my_json[0]["id"], 100024);
        EXPECT_EQ(my_json[1]["id"], 100027);
        EXPECT_EQ(my_json[2]["id"], 100050);
        EXPECT_EQ(my_json[0]["description"], "Futures contract for USD/RUB");
        EXPECT_EQ(my_json[1]["description"], "Futures contract for index RTS");
        EXPECT_EQ(my_json[2]["description"], "Futures contract for Gazprom shares");
    }
