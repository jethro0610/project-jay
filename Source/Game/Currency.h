#pragma once
class Currency {
public:
    Currency() {
        percent_ = 0;
        stocks_ = 0;
        maxStocks_ = 1;
    }
    static constexpr int PERCENT_TILL_STOCK = 1024 * 32;
    int percent_;
    int stocks_;
    int maxStocks_;

    void AddPercent(int amount = 1) { 
        if (stocks_ >= maxStocks_)
            return; 

        percent_ += amount; 
        if (percent_ >= PERCENT_TILL_STOCK) {
            percent_ = 0;
            stocks_++;
        }
    }
};
