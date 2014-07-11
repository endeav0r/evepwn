OBJS = mainWindow.o typesView.o order.o orderStore.o mapView.o \
       ordersView.o evePwnClient.o bidOfferView.o bidOfferWindow.o \
       eveDB.o spreadView.o spreadWindow.o curl.o

CXXFLAGS=-O2 -g -Werror -Wall -std=c++11
INCLUDE=`pkg-config gtkmm-3.0 --cflags`
LIB=`pkg-config gtkmm-3.0 --libs` -ljansson -lzmq -lmsgpack -lcurl

all : evepwn

evepwn : $(OBJS)
	$(CXX) -o ../evepwn main.cc $(OBJS) $(INCLUDE) $(CXXFLAGS) $(LIB)

test : $(OBJS)
	$(CXX) -o ../curlTest curlTest.cc $(OBJS) $(INCLUDE) $(CXXFLAGS) $(LIB)

%.o : %.cc %.h
	$(CXX) -c -o $@ $< $(INCLUDE) $(CXXFLAGS)

%.o : %.c %.h
	$(CXX) -c -o $@ $< $(INCLUDE) $(CXXFLAGS)

clean :
	rm -f *.o
	rm -f ../evepwn
	rm -f ../curlTest
