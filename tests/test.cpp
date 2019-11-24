#include <gtest/gtest.h>
#include <fstream>

#include "json.hpp"
#include "JsonE.hpp"

TEST(Json, ExampleJson)
{
    Json object("{\"lastname\" : \"Ivanov\",\"firstname\" : \"Ivan\",\"age\" : 25,\"islegal\" : false,\"marks\" : [4,5,5,5,2,3],\"address\" : {\"city\" : \"Moscow\",\"street\" : \"Vozdvijenka\"}}");

    EXPECT_EQ(std::any_cast<std::string>(object["lastname"]), "Ivanov");
    EXPECT_EQ(std::any_cast<bool>(object["islegal"]), false);
    EXPECT_EQ(std::any_cast<double>(object["age"]), 25);

    Json marks = std::any_cast<Json >(object["marks"]);
    EXPECT_EQ(std::any_cast<double>(marks[0]), 4);
    EXPECT_EQ(std::any_cast<double>(marks[1]), 5);

    Json address = std::any_cast<Json >(object["address"]);
    EXPECT_EQ(std::any_cast<std::string>(address["city"]), "Moscow");
    EXPECT_EQ(std::any_cast<std::string>(address["street"]), "Vozdvijenka");
}

TEST(JsonObject, object)
{
    Json json{ "{ \"grean\" : \"tea\" }" };
    EXPECT_EQ(json.is_object(), true);
    EXPECT_EQ(json.is_array(), false);

    EXPECT_EQ(std::any_cast<std::string>(json["grean"]), "tea");
}

TEST(JsonObject, inobject)
{
    Json json{ "{ \"my\" : { \"car\":   1230000 } }" };
    EXPECT_EQ(json.is_object(), true);
    EXPECT_EQ(json.is_array(), false);

    Json nested = std::any_cast<Json >(json["my"]);
    EXPECT_EQ(nested.is_object(), true);
    EXPECT_EQ(nested.is_array(), false);

    EXPECT_EQ(std::any_cast<double>(nested["car"]), 1230000);
}

TEST(JsonObject, arrays)
{
    Json json{ "{ \"play\" : [true,false,true]}" };
    EXPECT_EQ(json.is_object(), true);
    EXPECT_EQ(json.is_array(), false);

    Json nested = std::any_cast<Json >(json["play"]);
    EXPECT_EQ(nested.is_object(), false);
    EXPECT_EQ(nested.is_array(), true);

    EXPECT_EQ(std::any_cast<bool>(nested[0]), true);
    EXPECT_EQ(std::any_cast<bool>(nested[1]), false);
    EXPECT_EQ(std::any_cast<bool>(nested[2]), true);

}

TEST(JsonObject, manyinobj)
{
    Json json{ "{ \"blue\" : [\"red\",23.4,false], \"mum\": \"dad\" }" };
    EXPECT_EQ(json.is_object(), true);
    EXPECT_EQ(json.is_array(), false);

    Json nested = std::any_cast<Json >(json["blue"]);
    EXPECT_EQ(nested.is_object(), false);
    EXPECT_EQ(nested.is_array(), true);

    EXPECT_EQ(std::any_cast<std::string>(nested[0]), "red");
    EXPECT_EQ(std::any_cast<double>(nested[1]), 23.4);
    EXPECT_EQ(std::any_cast<bool>(nested[2]), false);

    EXPECT_EQ(std::any_cast<std::string>(json["mum"]), "dad");
}

TEST(JsonArray, simple)
{
    Json json{ "[ true ]" };
    EXPECT_EQ(json.is_object(), false);
    EXPECT_EQ(json.is_array(), true);

    EXPECT_EQ(std::any_cast<bool>(json[0]), true);
}

TEST(JsonArray, hardarray)
{
    Json json{ "[true, \"kola\", 53, false]" };
    EXPECT_EQ(json.is_object(), false);
    EXPECT_EQ(json.is_array(), true);

    EXPECT_EQ(std::any_cast<bool>(json[0]), true);
    EXPECT_EQ(std::any_cast<std::string>(json[1]), "kola");
    EXPECT_EQ(std::any_cast<double>(json[2]), 53);
    EXPECT_EQ(std::any_cast<bool>(json[3]), false);
}

TEST(JsonArray, nospace)
{
    Json json{ "[1]" };

    EXPECT_EQ(json.is_object(), false);
    EXPECT_EQ(json.is_array(), true);

    EXPECT_EQ(std::any_cast<double>(json[0]), 1);
}

TEST(JsonArray, arraywithobjects)
{
    Json json{ "[{\"fanta\": \"sprite\"}, {\"price\": 777}, {\"pain\": false}]" };

    EXPECT_EQ(json.is_object(), false);
    EXPECT_EQ(json.is_array(), true);

    {
        Json nested = std::any_cast<Json >(json[0]);
        EXPECT_EQ(nested.is_object(), true);
        EXPECT_EQ(nested.is_array(), false);

        EXPECT_EQ(std::any_cast<std::string>(nested["fanta"]), "sprite");
    }

    {
        Json nested = std::any_cast<Json >(json[1]);
        EXPECT_EQ(nested.is_object(), true);
        EXPECT_EQ(nested.is_array(), false);

        EXPECT_EQ(std::any_cast<double>(nested["price"]), 777);
    }

    {
        Json nested = std::any_cast<Json >(json[2]);
        EXPECT_EQ(nested.is_object(), true);
        EXPECT_EQ(nested.is_array(), false);

        EXPECT_EQ(std::any_cast<bool>(nested["pain"]), false);
    }
}


TEST(JsonArray, array_with_anything)
{
    Json json{ "[true, 669,996,[ true, false, true ],\"dream\",{ \"price\" : 98765 },\"love\" ]" };

    EXPECT_EQ(json.is_object(), false);
    EXPECT_EQ(json.is_array(), true);


    EXPECT_EQ(std::any_cast<bool>(json[0]), true);
    EXPECT_EQ(std::any_cast<double>(json[1]), 669);


    EXPECT_EQ(std::any_cast<double>(json[2]), 996);

    {
        Json nested = std::any_cast<Json >(json[3]);

        EXPECT_EQ(nested.is_object(), false);
        EXPECT_EQ(nested.is_array(), true);


        EXPECT_EQ(std::any_cast<bool>(nested[0]), true);


        EXPECT_EQ(std::any_cast<bool>(nested[1]), false);


        EXPECT_EQ(std::any_cast<bool>(nested[2]), true);
    }

    EXPECT_EQ(std::any_cast<std::string>(json[4]), "dream");

    {
        Json nested = std::any_cast<Json >(json[5]);
        EXPECT_EQ(nested.is_object(), true);
        EXPECT_EQ(nested.is_array(), false);

        EXPECT_EQ(std::any_cast<double>(nested["price"]), 98765);
    }

    EXPECT_EQ(std::any_cast<std::string>(json[6]), "love");
}

TEST(JsonArray, ArrayNull)
{
Json json{ "[ null ]" };
EXPECT_EQ(json.is_object(), false);
EXPECT_EQ(json.is_array(), true);

EXPECT_EQ(json[0].has_value(), true);
}


TEST(JsonObject, exc1) {
    EXPECT_THROW(
            Json{"{,"},
            Except
   );
}

TEST(JsonObject, exc2) {
    EXPECT_THROW(
            Json{"{\"key\" 23"},
            Except
    );
}

TEST(JsonObject, exs3)
{
    EXPECT_THROW(
            Json{"{\"key\" : false \"key2\""},
            Except
    );
}

TEST(JsonObject, exc3) {
    EXPECT_THROW(
            Json{"[\"key\" 23]"},
            Except
    );
}

TEST(Json, ParseFile)
{
 auto json = Json::parseFile("../tests/testparsefile.json");

    EXPECT_EQ(json.is_object(), false);
    EXPECT_EQ(json.is_array(), true);
}

TEST(Json, ParseFileException)
{
    EXPECT_THROW(
            Json::parseFile("file_not_existing"),
            Except
    );
}


TEST(Json, Parse)
{
    
    auto json = Json::parse("{\"key\" : \"value\"}");

    EXPECT_EQ(json.is_object(), true);
    EXPECT_EQ(json.is_array(), false);
}

TEST(JsonArray, array_in_array)
{
    Json json{ "[ false, [[false], 1, 3], true]" };

    EXPECT_EQ(json.is_object(), false);
    EXPECT_EQ(json.is_array(), true);
}

TEST(JsonArray,  obj_in_obj)
{
    Json json{ "[ false, { \"key\" : {\"key2\" : false}}, 1, 3, true]" };

    EXPECT_EQ(json.is_object(), false);
    EXPECT_EQ(json.is_array(), true);
}
