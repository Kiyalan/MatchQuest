#pragma once
#ifndef SERIALIZATION_H
#define SERIALIZATION_H
// #include <QDataStream>
// #include <QMetaType>
#include <QObject>

class QDataStream;
class SingleMatcherController;
class SinglePlayerLevel;
class DualMatcherController;
class DualPlayerLevel;

class Serialization: public QObject
{
    Q_OBJECT
public:
    static Serialization* getInstance();
public:
    void SerializeSinglePlayer(const SingleMatcherController* control, const SinglePlayerLevel* level, QDataStream& outStream) const;
    void DeserializeSinglePlayer(SingleMatcherController* control, SinglePlayerLevel* level, QDataStream& inStream) const;
    void SerializeDualPlayer(const DualMatcherController* control, const DualPlayerLevel* level, QDataStream& outStream) const;
    void DeserializeDualPlayer(DualMatcherController* control, DualPlayerLevel* level, QDataStream& inStream) const;
};

#endif // SERIALIZATION_H
