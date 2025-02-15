#ifndef MOCKVALUE_H
#define MOCKVALUE_H

template<typename T1>
class SingleMockValue
{
public:
	SingleMockValue(T1 data1_) : mock1(data1_){}
        SingleMockValue(){}                               // put here b'cause no matching function for call to 'SingleMockValue<unsigned char>::SingleMockValue()'

        T1 mock1;

};

template<typename T1, typename T2, typename T3>
class TripleMockValue
{
public:
	TripleMockValue(T1 data1_, T2 data2_, T3 data3_) : mock1(data1_), mock2(data2_), mock3(data3_) {}
        
        T1 mock1;
        T2 mock2;
        T3 mock3;

};

#endif