#include "morseencoder.h"

#include <QFile>
#include <QTextStream>

MorseEncoder::MorseEncoder()
{
}

bool MorseEncoder::loadSymbols(const QString& filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if (line.isEmpty())
            continue;

        QStringList parts = line.split(";");

        if (parts.size() != 3)
            continue;

        Symbol symbol;

        symbol.language = parts[0];
        symbol.character = parts[1];
        symbol.morseCode = parts[2];

        symbols.append(symbol);

        symbolToMorseMap.insert(
            symbol.character,
            symbol.morseCode);

        morseToSymbolMap.insert(
            symbol.morseCode,
            symbol.character);
    }

    return true;
}

QString MorseEncoder::symbolToMorse(
    const QString& symbol) const
{
    return symbolToMorseMap.value(symbol);
}

QString MorseEncoder::morseToSymbol(
    const QString& morse) const
{
    return morseToSymbolMap.value(morse);
}

QString MorseEncoder::encode(
    const QString& text) const
{
    QString result;

    for (QChar ch : text.toUpper())
    {
        QString morse =
            symbolToMorseMap.value(QString(ch));

        if (!morse.isEmpty())
        {
            result += morse + " ";
        }
    }

    return result.trimmed();
}

QString MorseEncoder::decode(
    const QString& morseText) const
{
    QString result;

    QStringList parts =
        morseText.split(" ",
                        Qt::SkipEmptyParts);

    for (const QString& code : parts)
    {
        result += morseToSymbolMap.value(code);
    }

    return result;
}

QList<Symbol> MorseEncoder::getSymbols() const
{
    return symbols;
}

QString MorseEncoder::decodeForLanguage(
    const QString& morseText,
    const QString& language) const
{
    QStringList parts =
        morseText.split(
            " ",
            Qt::SkipEmptyParts);

    QString result;

    for (const QString& code : parts)
    {
        for (const Symbol& symbol : symbols)
        {
            if (symbol.morseCode != code)
                continue;

            if (symbol.language == "NUM")
            {
                result += symbol.character;
                break;
            }

            if (language == "English"
                && symbol.language == "EN")
            {
                result += symbol.character;
                break;
            }

            if (language == "Ukrainian"
                && symbol.language == "UA")
            {
                result += symbol.character;
                break;
            }
        }
    }

    return result;
}
