#ifndef MORSEENCODER_H
#define MORSEENCODER_H

#include <QMap>
#include <QString>
#include <QList>

#include "models/symbol.h"

class MorseEncoder
{
public:
    MorseEncoder();

    bool loadSymbols(const QString& filePath);

    QString symbolToMorse(const QString& symbol) const;
    QString morseToSymbol(const QString& morse) const;

    QString encode(const QString& text) const;
    QString decode(const QString& morseText) const;

    QList<Symbol> getSymbols() const;

    QString decodeForLanguage(
        const QString& morseText,
        const QString& language) const;

private:
    QList<Symbol> symbols;

    QMap<QString, QString> symbolToMorseMap;
    QMap<QString, QString> morseToSymbolMap;
};

#endif
