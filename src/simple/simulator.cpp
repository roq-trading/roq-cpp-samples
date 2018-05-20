//
// Created by parallels on 5/19/18.
//

#include "simulator.h"

namespace examples {
namespace simple {

    void Simulator::update(const common::MarketData& market_data) {
        _order_book.insert(
            std::make_pair(
                    market_data.symbol,
                    std::move(InstrumentOrderBook(market_data.depth, market_data.depth+roq::MAX_DEPTH)))
        );
    }

    void Simulator::match(
            const char* symbol,
            roq::Side side,
            double quantity,
            double price,
            SimulatorCallback callback
    ) {
        const InstrumentOrderBook& order_book = _order_book[symbol];

        if (order_book.size() <= 0) {
            return;
        }

        if (side == roq::Side::Buy) {
            // Check ask side, ask
            for (size_t i = 0; i < order_book.size() && quantity > 0.0; i++) {
                const roq::Layer& layer = order_book[i];
                if (layer.ask_price <= price) {
                    // Good thing is that we don't need to update order book.
                    if (layer.ask_quantity >= quantity) {
                        callback(symbol, side, quantity, layer.ask_price);
                        break;
                    } else {
                        // layer does not have enough quantity
                        callback(symbol, side, layer.ask_quantity, layer.ask_price);
                        quantity -= layer.ask_quantity;
                    }
                } else {
                    break;
                }
            }
        } else if (side == roq::Side::Sell) {
            // Check bid side
            for (size_t i = 0; i < order_book.size() && quantity > 0.0; i++) {
                const roq::Layer& layer = order_book[i];
                if (layer.bid_price >= price) {
                    if (layer.bid_quantity >= quantity) {
                        callback(symbol, side, quantity, layer.bid_price);
                        break;
                    } else {
                        // layer does not have enough quantity
                        callback(symbol, side, layer.bid_quantity, layer.bid_price);
                        quantity -= layer.bid_quantity;
                    }
                } else {
                    break;
                }
            }
        }
    }

}
}