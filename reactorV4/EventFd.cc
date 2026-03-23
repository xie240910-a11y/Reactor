#include "EventFd.h"
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <sys/eventfd.h>

EventFd::EventFd(EventFdCallback &&cb)
: evtfd_(createEventFd())
, cb_(std::move(cb))
, isStarted_(false)
{ }

EventFd::EventFd(const EventFdCallback &cb)
: evtfd_(createEventFd())
, cb_(cb)
, isStarted_(false)
{ }

EventFd::~EventFd()
{
    close(evtfd_);
}

void EventFd::start()
{
    struct pollfd pfd;
    pfd.events = POLLIN;
    pfd.fd = evtfd_;

    isStarted_ = true;
    while (isStarted_) {
        int nready = poll(&pfd, 1, 5000);
        if (nready == -1 && errno == EINTR) {
            continue;
        } else if (nready == -1) {
            perror("poll");
        } else if (nready == 0) {
            printf(">>poll time_out!!!\n");
        } else {
            if  (pfd.revents & POLLIN) {
                handleRead();
                if (cb_) {
                    cb_();
                }
            }
        }
    }
}

void EventFd::stop()
{
    isStarted_ = false;
}

void EventFd::wakeup()
{
    uint64_t one = 1;
    ssize_t ret = write(evtfd_, &one, sizeof(uint64_t));
    if (ret != sizeof(uint64_t)) {
        perror("wakeup");
        return;
    }
}

int EventFd::createEventFd()
{
    int fd = eventfd(0, 0);
    if (fd < 0) {
        perror("createEventFd");
        return -1;
    }
    return fd;
}
void EventFd::handleRead()
{
    uint64_t one = 1;
    ssize_t ret = read(evtfd_, &one, sizeof(uint64_t));
    if (ret != sizeof(uint64_t)) {
        perror("handleRead");
        return;
    }
}
