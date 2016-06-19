#ifndef COMMANDS
#define COMMANDS

#include <QString>

static const int loginCommand = 1;
static const int successLogin = 2;
static const int error = 3;
static const int getItemGroups = 4;
static const int getItemsFromGroup = 5;
static const int getItemCharValues = 6;
static const int placeOrd = 7;
static const int addItemsToOrd = 8;
static const int orderSuccPlaces = 9;
static const int getPermissions = 10;
static const int getCharNames = 11;
static const int editItem = 12;
static const int addItem = 13;
static const int delItem = 14;
static const int successDelItem = 15;
static const int successAddItem = 16;
static const int addUserC = 17;
static const int modUserC = 18;
static const int delUserc = 19;
static const int succUserMod = 20;
static const int reqUserList = 21;
static const int reqOrderList = 22;
static const int getCharLinks = 23;



static const int wronglogpass = 1;
static const int nopermisssions = 2;
static const int undefinederror = 69;


static const int valuesToShow = 1;
static const int valuesToEdit = 2;

static const QString adminP = "admin";
static const QString sellerP = "seller";
static const QString userP = "user";

enum permissions {
    user,
    seller,
    admin
};

struct itemChars{
    QString charname = "";
    QString charValue = "";
    QString charUnits = "";
    int charId = -1;
    int itemId = -1;
};

enum messType {
    data,
    connectMes,
    rsaOpenkey,
    aesKey

};

struct chars{
    int charId = -1;
    QString charname = "";
};

struct links{
    int char1 = -1;
    QString val = "";
    int char2 = -1;
};

#endif // COMMANDS

