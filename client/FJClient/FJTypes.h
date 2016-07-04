#ifndef FJTYPES_H
#define FJTYPES_H

#include <memory>
#include <QVariant>

template<typename T> struct FJDeclarePtrs {
    typedef std::shared_ptr< T >     SharedPtr;
    typedef std::weak_ptr< T >       Ptr;
};

#define FJ_DECLARE_PTRS(type)    \
    class type;                       \
    typedef FJDeclarePtrs<class type>::Ptr type##Ptr;             \
    typedef FJDeclarePtrs<class type>::SharedPtr type##SharedPtr;


template <typename TYPE>
TYPE FJType_GetValueFromMap(const QString& key,
                            const QVariantMap& dict,
                            const TYPE& defaultValue)
{
    QVariantMap::const_iterator it = dict.find(key);
    if ((it != dict.end()) && (it.key() == key) && it->canConvert<TYPE>()) {
        return it->value<TYPE>();
    }
    return defaultValue;
}

#endif // FJTYPES_H
