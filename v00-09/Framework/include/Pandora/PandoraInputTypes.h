/**
 *  @file   PandoraPFANew/Framework/include/Pandora/PandoraInputTypes.h
 * 
 *  @brief  Header file for pandora input types and associated external typedefs exposed via the PandoraApi
 * 
 *  $Log: $
 */
#ifndef PANDORA_INPUT_TYPES_H
#define PANDORA_INPUT_TYPES_H 1

#include "Objects/CartesianVector.h"
#include "Objects/TrackState.h"

#include "Pandora/PandoraInternal.h"
#include "Pandora/PdgTable.h"
#include "Pandora/StatusCodes.h"

#include "Xml/tinyxml.h"

#include <cmath>
#include <vector>

namespace pandora
{

/**
 *  @brief  Calorimeter hit type enum
 */
enum HitType
{
    INNER_DETECTOR,
    ECAL,
    HCAL,
    MUON
};

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  Granularity enum
 */
enum Granularity
{
    VERY_FINE,
    FINE,
    COARSE,
    VERY_COARSE
};

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  DetectorRegion enum
 */
enum DetectorRegion
{
    BARREL,
    ENDCAP
};

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  EnergyCorrectionType enum
 */
enum EnergyCorrectionType
{
    HADRONIC,
    ELECTROMAGNETIC
};

//------------------------------------------------------------------------------------------------------------------------------------------

/**
 *  @brief  PandoraInputType template class
 */
template <typename T>
class PandoraInputType
{
public:
    /**
     *  @brief  Default constructor
     */
    PandoraInputType();

    /**
     *  @brief  Destructor
     */
    ~PandoraInputType();

    /**
     *  @brief  Constructor
     *
     *  @param  t the initial value
     */
    PandoraInputType(const T &t);

    /**
     *  @brief  Copy constructor (EXPENSIVE WITH LARGE OBJECTS)
     * 
     *  @param  rhs the initial pandora type
     */
    PandoraInputType(const PandoraInputType<T> &rhs);

    /**
     *  @brief  Set the value held by the pandora type
     *
     *  @param  t the value
     */
    void Set(const T &t);

    /**
     *  @brief  Get the value held by the pandora type
     *
     *  @return the value
     */
    const T &Get() const;

    /**
     *  @brief  Reset the pandora type
     */   
    void Reset();

    /**
     *  @brief  Whether the pandora type is initialized
     *
     *  @return boolean
     */
    bool IsInitialized() const;

    /**
     *  @brief  Assignment operator for the pandora type
     * 
     *  @return whether assignment succeeded
     */
    bool operator= (const T &rhs); 

    /**
     *  @brief  Assignment operator for parent pandora type (EXPENSIVE WITH LARGE OBJECTS)
     * 
     *  @throw  status code exception
     */   
    bool operator= (const PandoraInputType<T> &rhs);

private:
    /**
     *  @brief  Whether pandora type is valid (not inf or nan)
     * 
     *  @return boolean
     */
    bool IsValid(const T &t) const;

    T      *m_pValue;           ///< Address of the actual value being held by the pandora type
    bool    m_isInitialized;    ///< Whether the pandora type is initialized
};

//------------------------------------------------------------------------------------------------------------------------------------------

typedef PandoraInputType<unsigned int> InputUInt;
typedef PandoraInputType<int> InputInt;
typedef PandoraInputType<float> InputFloat;
typedef PandoraInputType<void *> InputAddress;
typedef PandoraInputType<bool> InputBool;
typedef PandoraInputType<HitType> InputHitType;
typedef PandoraInputType<DetectorRegion> InputDetectorRegion;
typedef PandoraInputType<unsigned int> InputPseudoLayer;

typedef PandoraInputType<CartesianVector> InputCartesianVector;
typedef PandoraInputType<TrackState> InputTrackState;
typedef std::vector<TrackState> InputTrackStateList;

typedef std::vector<const void *> CaloHitAddressList, TrackAddressList;
typedef std::vector<CaloHitAddressList> ClusterAddressList;

typedef void (EnergyCorrectionFunction)(const Cluster *const, float &);
typedef bool (ParticleIdFunction)(const Cluster *const);
typedef pandora::StatusCode (SettingsFunction)(const TiXmlHandle xmlHandle);

typedef std::vector<EnergyCorrectionFunction *> EnergyCorrectionFunctionVector;

//------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline PandoraInputType<T>::PandoraInputType() :
    m_pValue(NULL),
    m_isInitialized(false)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline PandoraInputType<T>::~PandoraInputType()
{
    if (NULL != m_pValue)
        delete m_pValue;
}

//------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline PandoraInputType<T>::PandoraInputType(const T &t) :
    m_pValue(new T(t)),
    m_isInitialized(true)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline PandoraInputType<T>::PandoraInputType(const PandoraInputType<T> &rhs) :
    m_pValue(NULL),
    m_isInitialized(rhs.m_isInitialized)
{
    if (rhs.m_isInitialized)
        m_pValue = new T(*rhs.m_pValue);
}

//------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline void PandoraInputType<T>::Set(const T &t)
{
    if (m_isInitialized)
        delete m_pValue;

    if (!this->IsValid(t))
        throw StatusCodeException(STATUS_CODE_INVALID_PARAMETER);

    m_pValue = new T(t);
    m_isInitialized = true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline const T &PandoraInputType<T>::Get() const
{
    if (!m_isInitialized)
        throw StatusCodeException(STATUS_CODE_NOT_INITIALIZED);

    return *m_pValue;
}

//------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline void PandoraInputType<T>::Reset()
{
    if (NULL != m_pValue)
        delete m_pValue;

    m_isInitialized = false;
    m_pValue = NULL;
}

//------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline bool PandoraInputType<T>::IsInitialized() const
{
    return m_isInitialized;
}

//------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline bool PandoraInputType<T>::operator= (const T &rhs)
{
    this->Set(rhs);
    return m_isInitialized;
}

//------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline bool PandoraInputType<T>::operator= (const PandoraInputType<T> &rhs)
{
    if (this == &rhs)
        return m_isInitialized;

    if (rhs.m_isInitialized)
    {
        this->Set(*rhs.m_pValue);
    }
    else
    {
        this->Reset();
    }

    return m_isInitialized;
}

//------------------------------------------------------------------------------------------------------------------------------------------

#ifdef _MSC_VER
    #include <float.h>
    #define IS_NAN _isnan
    #define IS_INF !_finite
#else
    #define IS_NAN std::isnan
    #define IS_INF std::isinf
#endif

template <typename T>
inline bool PandoraInputType<T>::IsValid(const T &t) const
{
    return !(IS_NAN(t) || IS_INF(t));
}

template <>
inline bool PandoraInputType<void *>::IsValid(void *const &) const
{
    return true;
}

template <>
inline bool PandoraInputType<HitType>::IsValid(const HitType &t) const
{
    return !(IS_NAN(static_cast<unsigned int>(t)) || IS_INF(static_cast<unsigned int>(t)));
}

template <>
inline bool PandoraInputType<DetectorRegion>::IsValid(const DetectorRegion &t) const
{
    return !(IS_NAN(static_cast<unsigned int>(t)) || IS_INF(static_cast<unsigned int>(t)));
}

template <>
inline bool PandoraInputType<CartesianVector>::IsValid(const CartesianVector &t) const
{
    return !(IS_NAN(t.GetX()) || IS_NAN(t.GetY()) || IS_NAN(t.GetZ()) ||
        IS_INF(t.GetX()) || IS_INF(t.GetY()) || IS_INF(t.GetZ()));
}

template <>
inline bool PandoraInputType<TrackState>::IsValid(const TrackState &t) const
{
    const CartesianVector &p(t.GetPosition());
    const CartesianVector &m(t.GetMomentum());

    return !(IS_NAN(p.GetX()) || IS_NAN(p.GetY()) || IS_NAN(p.GetZ()) ||
        IS_INF(p.GetX()) || IS_INF(p.GetY()) || IS_INF(p.GetZ()) ||
        IS_NAN(m.GetX()) || IS_NAN(m.GetY()) || IS_NAN(m.GetZ()) ||
        IS_INF(m.GetX()) || IS_INF(m.GetY()) || IS_INF(m.GetZ()));
}

} // namespace pandora

#endif // #ifndef PANDORA_INPUT_TYPES_H
