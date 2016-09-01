#ifndef ITEMDATA_H
#define ITEMDATA_H


#include <QString>


class SItemData
{
public:
    enum SItemDataType
    {
        Stock,
        Index,
        Indicator,
        Strategy,
        Root
    };

    SItemData();
    QString getName();
    SItemDataType getType();
protected:
    void setName(QString newName);
    void setType(SItemDataType newType);

private:
    QString name;
    SItemDataType type;


};

#endif // ITEMDATA_H
