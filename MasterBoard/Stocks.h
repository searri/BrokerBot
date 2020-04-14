struct stock
{
    short stockName;
    short quantity;

    // How much was the stock worth when it was purchased PER SHARE
    short startVal;

    // How much is the stock worth PER SHARE
    short currVal;

    // How much has the stock changed PER SHARE
    short amtChange;

    // This is the dividend per 10 SHARES
    short dividend;

    // Internal attribute for purchasing
    bool canBuy;    
};
