#ifndef SAMPLE_H
#define SAMPLE_H

#include <QString>
#include <QDataStream>

typedef struct
{
QString message;
QString sender;

} Sample;

QDataStream & operator <<( QDataStream & stream, const Sample &sample);
QDataStream & operator >>(QDataStream & stream, Sample & sample);

#endif // SAMPLE_H
