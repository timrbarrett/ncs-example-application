#pragma once

#include "message_enum.h"

//#include "utility"
#include "../etl/algorithm.h" // etl::move
#include "../etl/deque.h" // from RX libs
#include "../etl/message_bus.h" // from ETL
#include "MockValue.h" 
// uMI is unique Message Index

// single type message
template<typename T, const int uMI>
class SingleMessage : public etl::message<uMI>
{

public:
	SingleMessage<T, uMI>(T data1_) : data1(data1_) {} // constructor

	SingleMessage<T, uMI>(SingleMessage<T, uMI>&& rhs) : data1(rhs.data1) {} // simplest move constructor

	SingleMessage<T, uMI>(const SingleMessage<T, uMI>& rhs) : data1(rhs.data1) {} // simplest copy constructor

	SingleMessage<T, uMI>& operator= (const SingleMessage<T, uMI>& other) {
		if (this != &other) {
			//https://www.internalpointers.com/post/c-rvalue-references-and-move-semantics-beginners
			data1 = other.data1;
		}
		return *this;
	} // copy assignment

	~SingleMessage<T, uMI>() {

	}

	T data1; 

};

// three type Message

template<typename T1, typename T2, typename T3, const int uMI>
class TripleMessage : public etl::message<uMI>
{

public:
	TripleMessage(T1 data1_, T2 data2_, T3 data3_) : data1(data1_), data2(data2_), data3(data3_) {} // constructor

	TripleMessage(TripleMessage&& rhs) : data1(rhs.data1), data2(rhs.data2), data3(rhs.data3) {} // simplest move constructor

	TripleMessage(const TripleMessage& rhs) : data1(rhs.data1), data2(rhs.data2), data3(rhs.data3) {} // simplest copy constructor

	TripleMessage& operator= (const TripleMessage& other) {
		if (this != &other) {
			//https://www.internalpointers.com/post/c-rvalue-references-and-move-semantics-beginners
			data1 = other.data1;
			data2 = other.data2;
			data3 = other.data3;
		}
		return *this;
	} // copy assignment

	~TripleMessage() {

	}

       //oid setMock(T1 mock1_, T2 mock2_, T3 mock3_) {
       // mock1 = mock1_;
       // mock2 = mock2_;
       // mock3 = mock3_;
       // }
      /*
        void create(TripleMockValue<T1, T2, T3> mock) {
          data1=mock.mock1;
          data2=mock.mock2;
          data3=mock.mock3;
        }
       */   
	T1 data1;
	T2 data2;
	T3 data3;

        bool trueValue;
};

//template class SingleMessage<uint8_t, MESSAGE_CHAIN_LINK>;

/*
// 1) The milli message
struct MilliReading : public etl::messageTRB<MESSAGE_MILLI1>
{

	MilliReading(uint32_t milli_) : milli(milli_) {
		//globalbp.print("<<ctr:"); 	globalbp.print(milli);	globalbp.println(">>");
	} // constructor

	MilliReading(MilliReading&& rhs) : milli(rhs.milli) {
		//globalbp.print("<<movctr:"); 	globalbp.print(milli);	globalbp.println(">>");
	} // simplest move constructor

	MilliReading(const MilliReading& rhs) : milli(rhs.milli) { // <--------- const added 15th Mar 11:30
	  //globalbp.print("<<cpyctr:"); 	globalbp.print(milli);	globalbp.println(">>");
	} // simplest copy constructor

	MilliReading& operator= (const MilliReading& other) {
		if (this != &other) {
			//https://www.internalpointers.com/post/c-rvalue-references-and-move-semantics-beginners
			milli = other.milli;
		}
		//globalbp.print("<<cpyasn:"); 	globalbp.print(milli);	globalbp.println(">>");
		return *this;
	} // copy assignment

	~MilliReading() {
		//globalbp.print("<<destructor:"); 	globalbp.print(milli);	globalbp.println(">>");
	}

	uint32_t milli;
};

*/