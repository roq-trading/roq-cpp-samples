//
// Created by parallels on 5/19/18.
//

#ifndef CXX_SIMULATOR_H
#define CXX_SIMULATOR_H

#include <roq/api.h>

#include "common/base_strategy.h"
#include "common/market_data.h"

namespace examples {
namespace simple {
    class Simulator {
    public:
        typedef std::vector<roq::Layer> InstrumentOrderBook;
        typedef std::unordered_map<const char*, InstrumentOrderBook> OrderBook;
        typedef std::function<void(const char* symbol, roq::Side side, double price, double quantity)> SimulatorCallback;

        Simulator() {}

        void update(const common::MarketData& market_data);
        void match(const char* symbol, roq::Side side, double quantity, double price, SimulatorCallback callback);

    private:
        OrderBook _order_book;
    };
}
}



#endif //CXX_SIMULATOR_H
