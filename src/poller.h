#ifndef POLLER_H
#define POLLER_H

#include"util.h"
#include <map>

namespace handy {
    class Channel;

    class PollerBase : private noncopyable {
    public:
        void AddChannel(Channel *ch);

        void RemoveChannel(Channel *ch);

        void UpdateChannel(Channel *ch);

        void loopOnce(int waitMs);

    private:
        std::map<int, handy::Channel *> _channels;
    };

} // namespace handy



#endif