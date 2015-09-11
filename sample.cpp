#include "sample.h"



QDataStream & operator << (QDataStream & stream, const Sample & sample){

    return stream <<sample.message <<sample.sender;
}
QDataStream & operator >> (QDataStream & stream, Sample & sample){

    return stream >> sample.message >>sample.sender;
}
