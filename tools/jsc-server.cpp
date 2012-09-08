/* For sockaddr_in */
#include <netinet/in.h>
/* For socket functions */
#include <sys/socket.h>
/* For fcntl */
#include <fcntl.h>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string>

#define MAX_LINE 16384

#include "jsc.h"
using namespace jsc;

Model model;
string format = "segment";
string table_mode = "both";
bool label = true;

void readcb(struct bufferevent *bev, void *ctx) {
  struct evbuffer *input, *output;
  char *line;
  size_t n;
  input = bufferevent_get_input(bev);
  output = bufferevent_get_output(bev);

  Decoder decoder(model);

  while ((line = evbuffer_readln(input, &n, EVBUFFER_EOL_LF))) {
    vector<Node> result;
    decoder.Decode((string)line, result, label);
    stringstream ss;

    if (format == "debug") {
      ss << ToStringDebug(result) << endl;
      ss << ToStringPlain(result) << endl;
    } else if (format == "plain") {
      ss << ToStringPlain(result) << endl;
    } else if (format == "segment") {
      ss << ToStringSegment(result) << endl;
    }

    evbuffer_add(output, ss.str().c_str(), ss.str().size());
    evbuffer_add(output, "\n", 1);
    free(line);
  }

  if (evbuffer_get_length(input) >= MAX_LINE) {
    /* Too long; just process what there is and go on so that the buffer
     * doesn't grow infinitely long. */
    char buf[1024];
    while (evbuffer_get_length(input)) {
      evbuffer_remove(input, buf, sizeof(buf));
      vector<Node> result;
      decoder.Decode((string)buf, result, false);
      string format = ToStringPlain(result);

      evbuffer_add(output, format.c_str(), format.size());
      evbuffer_add(output, "\n", 1);
    }
  }
}

void errorcb(struct bufferevent *bev, short error, void *ctx) {
  if (error & BEV_EVENT_EOF) {
    /* connection has been closed, do any clean up here */
  } else if (error & BEV_EVENT_ERROR) {
    /* check errno to see what error occurred */
  } else if (error & BEV_EVENT_TIMEOUT) {
    /* must be a timeout event handle, handle it */
  }
  bufferevent_free(bev);
}

void do_accept(evutil_socket_t listener, short event, void *arg) {
  struct event_base *base = (event_base*) arg;
  struct sockaddr_storage ss;
  socklen_t slen = sizeof(ss);
  int fd = accept(listener, (struct sockaddr*)&ss, &slen);
  if (fd < 0) {
    perror("accept");
  } else if (fd > FD_SETSIZE) {
    close(fd);
  } else {
    struct bufferevent *bev;
    evutil_make_socket_nonblocking(fd);
    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, readcb, NULL, errorcb, NULL);
    bufferevent_setwatermark(bev, EV_READ, 0, MAX_LINE);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
  }
}

void run(int port) {
  evutil_socket_t listener;
  struct sockaddr_in sin;
  struct event_base *base;
  struct event *listener_event;

  base = event_base_new();
  if (!base)
    return;

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = 0;
  sin.sin_port = htons(port);

  listener = socket(AF_INET, SOCK_STREAM, 0);
  evutil_make_socket_nonblocking(listener);

#ifndef WIN32
  {
    int one = 1;
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
  }
#endif

  if (bind(listener, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
    perror("bind");
    return;
  }

  if (listen(listener, 16)<0) {
    perror("listen");
    return;
  }

  listener_event = event_new(base, listener, EV_READ|EV_PERSIST, do_accept, (void*)base);
  event_add(listener_event, NULL);
  event_base_dispatch(base);
}

int main(int argc, char **argv) {
  string prefix = "./";
  int port = 40714;
  int c;
  while ((c = getopt (argc, argv, "d:f:t:p:l")) != -1) {
    switch (c) {
      case 'd':
        prefix = optarg;
        break;
      case 'f':
        format = optarg;
        break;
      case 't':
        table_mode = optarg;
        break;
      case 'p':
        port = atoi(optarg);
        break;
      case 'l':
        label = false;
        break;
      case '?':
        cerr << "Unknown option -" << optopt << endl;
        return 1;
    }
  }

  if (!model.LoadFromBinary(prefix.c_str())) {
    cerr << "Model " << prefix << "is not found." << endl;
    return 0;
  }
  setvbuf(stdout, NULL, _IONBF, 0);
  run(port);
  return 0;
}
