#ifndef PDN_MESSAGES_H
/// \exclude
#define PDN_MESSAGES_H

#include <vector>
#include <map>
#include <stdint.h>
#include <math.h>

#include "RDIPD0Interface.h"

namespace sonardyne_edge
{

namespace PDN_messages
{

const float f32DegToRadFactor = M_PI / 180.0f;
    
// rotation operations
void mult3DMatrixVector(const float (&m)[3][3], int16_t (&v)[3]);
void rotx(const float& c, const float& s, int16_t (&v)[3]);
void roty(const float& c, const float& s, int16_t (&v)[3]);
void rotz(const float& c, const float& s, int16_t (&v)[3]);
   
enum EFrame
{
    BEAM = 1,
    INSTRUMENT = 2,
    VESSEL = 3,
    EARTH = 4
};


class RDIPD0Header : public RDIPD0Interface
{
public:
    RDIPD0Header();
    RDIPD0Header(const RDIPD0Header& msg);
    virtual ~RDIPD0Header();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return Id; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;
    int deserialise(const char* buf, const int bytes);    
    
    void set(uint16_t PD0EnsembleBytes, uint8_t PD0NumberDataTypes);
    uint16_t get_TotalEnsembleBytes();
    
    static const int SIZE_BYTES = 6;

    uint8_t HeaderId   = Id >> 0;
    uint8_t DataSource = Id >> 8;
    uint16_t EnsembleBytes;
    uint8_t Spare;
    uint8_t NumberDataTypes;

    std::vector<uint16_t> Indices;    
};

class RDIFixedLeader : public RDIPD0Interface
{
public:
    RDIFixedLeader();
    RDIFixedLeader(const RDIFixedLeader& msg);
    virtual ~RDIFixedLeader();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return FixedLeaderID; }    
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;
    int deserialise(const char* buf, const int bytes);            
    
    static const int SIZE_BYTES = 59;

    const uint16_t FixedLeaderID = Id;      // 2
    uint8_t CpuFirmwareVer;                 // 3
    uint8_t CpuFirmwareRev;                 // 4
    uint16_t SystemConfig;                  // 6
    uint8_t RealOrSim;                      // 7
    uint8_t LagLengthCm;                    // 8
    uint8_t NumberOfBeams;                  // 9
    uint8_t NumberOfCells;                  // 10
    uint16_t PingsPerEnsemble;              // 12
    uint16_t DepthCellLengthCm;             // 14
    uint16_t BlankAfterTransmitCm;          // 16
    uint8_t ProfilingMode;                  // 17
    uint8_t LowcorrThresh;                  // 18
    uint8_t CodeRepeats;                    // 19
    uint8_t PercentGood;                    // 20
    uint16_t ErrorVelMax;                   // 22
    uint8_t TPPMins;                        // 23
    uint8_t TPPSecs;                        // 24
    uint8_t TPPHunds;                       // 25
    uint8_t CoordTxfrm;                     // 26
    uint16_t HeadingAlignment;              // 28
    uint16_t HeadingBias;                   // 30
    uint8_t SensorSource;                   // 31
    uint8_t SensorAvail;                    // 32
    uint16_t Bin1DistanceCm;                // 34
    uint16_t TxPulseLengthCm;               // 36
    uint16_t StartEndDepthCell;             // 38
    uint8_t FalseTargetThresh;              // 39
    uint8_t Spare1;                         // 40
    uint16_t TxLagDistanceCm;               // 42
    uint8_t Spares[8];                      // 50
    uint16_t WBSystemBandwidth;             // 52
    uint8_t SystemPower;                    // 53
    uint8_t Spare2;                         // 54
    uint32_t SerialNumber;                  // 58
    uint8_t BeamAngle;                      // 59
};

class RDIVariableLeader : public RDIPD0Interface
{
public:
    RDIVariableLeader();
    RDIVariableLeader(const RDIVariableLeader& msg);
    virtual ~RDIVariableLeader();
  
    static const uint16_t Id;    

    uint16_t get_ID() const { return VariableLeaderID; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
    
    static const int SIZE_BYTES = 65;

    const uint16_t VariableLeaderID = Id;       // 2
    uint16_t EnsembleNumber;                    // 4
    uint8_t RTCTSYear;                          // 5
    uint8_t RTCTSMonth;                         // 6
    uint8_t RTCTSDay;                           // 7
    uint8_t RTCTSHour;                          // 8
    uint8_t RTCTSMin;                           // 9
    uint8_t RTCTSSec;                           // 10
    uint8_t RTCTSHSec;                          // 11
    uint8_t EnsembleMSB;                        // 12
    uint16_t BITResult;                         // 14
    uint16_t SpeedOfSoundMs;                    // 16
    uint16_t DepthOfTxdrDm;                     // 18
    uint16_t HeadingCD;                         // 20
    int16_t PitchCD;                            // 22
    int16_t RollCD;                             // 24
    uint16_t SalinityPPT;                       // 26
    uint16_t TempCD;                            // 28
    uint8_t MptMins;                            // 29
    uint8_t MptSecs;                            // 30
    uint8_t MptHsecs;                           // 31
    uint8_t HeadingSdev;                        // 32
    uint8_t PitchSdev;                          // 33
    uint8_t RollSdev;                           // 34
    uint8_t AdcChan0;                           // 35
    uint8_t AdcChan1;                           // 36
    uint8_t AdcChan2;                           // 37
    uint8_t AdcChan3;                           // 38
    uint8_t AdcChan4;                           // 39
    uint8_t AdcChan5;                           // 40
    uint8_t AdcChan6;                           // 41
    uint8_t AdcChan7;                           // 42
    uint32_t StatusWord;                        // 46
    uint32_t PressureDP;                        // 50
    uint32_t PressureVarDP;                     // 54
    uint8_t Spare[3];                           // 57
    uint8_t RTCCentury;                         // 58
    uint8_t RTCYear;                            // 59
    uint8_t RTCMonth;                           // 60
    uint8_t RTCDay;                             // 61
    uint8_t RTCHour;                            // 62
    uint8_t RTCMin;                             // 63
    uint8_t RTCSec;                             // 64
    uint8_t RTCHSec;                            // 65    
};

class RDIVelocity : public RDIPD0Interface
{
public:
    RDIVelocity(uint8_t number_of_beams = 0,
                uint8_t number_of_cells = 0);
    RDIVelocity(const RDIVelocity& msg);
    virtual ~RDIVelocity();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return Header; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
    
    void set(const uint8_t& number_of_beams, const uint8_t& number_of_cells);    

    void transform(const EFrame& eFrame, const RDIFixedLeader* cFixedLeader, const RDIVariableLeader* cVariableLeader);
    void subtractVelocities(const std::vector<float> vesselMotionVelocities, const RDIFixedLeader* cFixedLeader, bool reverse);
    
    uint8_t _number_of_beams;
    uint8_t _number_of_cells;
    
    const uint16_t Header = Id;
    std::vector<int16_t> VelocitiesMms;

//private:
    std::vector<int16_t> BeamVelocitiesMms; // always keep these as a transform to another frame of reference will destroy the beam information
    
};

class RDIXC : public RDIPD0Interface
{
public:
    RDIXC(uint8_t number_of_beams,
          uint8_t number_of_cells);
    RDIXC(const RDIXC& msg);
    virtual ~RDIXC();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return Header; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
    
    void set(const uint8_t& number_of_beams, const uint8_t& number_of_cells);    
    
    uint8_t _number_of_beams;
    uint8_t _number_of_cells;
    
    const uint16_t Header = Id;
    std::vector<uint8_t> BeamXCs;
};

class RDIIntensity : public RDIPD0Interface
{
public:
    RDIIntensity(uint8_t number_of_beams,
                 uint8_t number_of_cells);
    RDIIntensity(const RDIIntensity& msg);
    virtual ~RDIIntensity();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return Header; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
    
    void set(const uint8_t& number_of_beams, const uint8_t& number_of_cells);    
    std::vector<uint8_t>& get_BeamIntensities();
    
    uint8_t _number_of_beams;
    uint8_t _number_of_cells;
    
    const uint16_t Header = Id;
    std::vector<uint8_t> BeamIntensities;
};

class RDIPrctGood : public RDIPD0Interface
{
public:
    RDIPrctGood(uint8_t number_of_beams,
                uint8_t number_of_cells);
    RDIPrctGood(const RDIPrctGood& msg);
    virtual ~RDIPrctGood();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return Header; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
    
    void set(const uint8_t& number_of_beams, const uint8_t& number_of_cells);    
    
    uint8_t _number_of_beams;
    uint8_t _number_of_cells;
    
    const uint16_t Header = Id;
    std::vector<uint8_t> BeamPrctGood;
};

class RDIBottomTrack : public RDIPD0Interface
{
public:
    RDIBottomTrack();
    RDIBottomTrack(const RDIBottomTrack& msg);
    virtual ~RDIBottomTrack();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return BottomTrackID; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
    
    static const int SIZE_BYTES = 85;
        
    const uint16_t BottomTrackID = Id;
    uint16_t BTPingsPerEnsemble;
    uint16_t BTDelayBeforeReaquire;
    uint8_t BTCorrMagMin;
    uint8_t BTEvalAmpMin;
    uint8_t BTPercentGood;
    uint8_t BTMode;
    uint16_t BTErrVelMax;
    uint32_t BTReserved;
    uint16_t BTBeam0RangeCm;
    uint16_t BTBeam1RangeCm;
    uint16_t BTBeam2RangeCm;
    uint16_t BTBeam3RangeCm;
    int16_t BTBeam0VelMms;
    int16_t BTBeam1VelMms;
    int16_t BTBeam2VelMms;
    int16_t BTBeam3VelMms;
    uint8_t BTBeam0Corr;
    uint8_t BTBeam1Corr;
    uint8_t BTBeam2Corr;
    uint8_t BTBeam3Corr;
    uint8_t BTBeam0Amp;
    uint8_t BTBeam1Amp;
    uint8_t BTBeam2Amp;
    uint8_t BTBeam3Amp;
    uint8_t BTBeam0PercentGood;
    uint8_t BTBeam1PercentGood;
    uint8_t BTBeam2PercentGood;
    uint8_t BTBeam3PercentGood;
    uint16_t RefLayerMinDm;
    uint16_t RefLayerNearDm;
    uint16_t RefLayerFarDm;
    int16_t BTRefLayerVelBeam0Mms;
    int16_t BTRefLayerVelBeam1Mms;
    int16_t BTRefLayerVelBeam2Mms;
    int16_t BTRefLayerVelBeam3Mms;
    uint8_t BTBeam0RefLayerCorr;
    uint8_t BTBeam1RefLayerCorr;
    uint8_t BTBeam2RefLayerCorr;
    uint8_t BTBeam3RefLayerCorr;
    uint8_t BTBeam0RefLayerIntensity;
    uint8_t BTBeam1RefLayerIntensity;
    uint8_t BTBeam2RefLayerIntensity;
    uint8_t BTBeam3RefLayerIntensity;
    uint8_t BTRefPercentGoodBeam0;
    uint8_t BTRefPercentGoodBeam1;
    uint8_t BTRefPercentGoodBeam2;
    uint8_t BTRefPercentGoodBeam3;
    uint16_t BTMaxDepthDm;
    uint8_t BTRSSIBeam0;
    uint8_t BTRSSIBeam1;
    uint8_t BTRSSIBeam2;
    uint8_t BTRSSIBeam3;
    uint8_t Gain;
    uint8_t BTBeam0RangeCmMbs;
    uint8_t BTBeam1RangeCmMbs;
    uint8_t BTBeam2RangeCmMbs;
    uint8_t BTBeam3RangeCmMbs;
    uint32_t Reserved;
};

const int RDINMEABINARYHEADERSIZE = 14;
const int RDINMEABINARYTIMESIZE   = 8;
const uint16_t RDINMEABinaryID    = 0x2022;

class RDINMEAGGABinary : public RDIPD0Interface
{
public:
    
    RDINMEAGGABinary();
    RDINMEAGGABinary(const RDINMEAGGABinary& msg);
    virtual ~RDINMEAGGABinary();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return SID; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
    
    static const uint16_t SIZE_BYTES = 71;        
    static const int      SZ_HEADER_BYTES = 7;
    static const int      SZ_UTC_BYTES = 10;

    static const uint16_t SID = 104;
    
    const uint16_t IdCode = Id;     // 
    char szHeader[SZ_HEADER_BYTES]; // 7
    char szUTC[SZ_UTC_BYTES];       // 17
    double dLatitude;               // 25
    char tcNS;                      // 26
    double dLongitude;              // 34
    char tcEW;                      // 35
    uint8_t ucQuality;              // 36
    uint8_t ucNmbSat;               // 37
    float fHDOP;                    // 41
    float fAltitude;                // 45
    char tcAltUnit;                 // 46
    float fGeoid;                   // 50
    char tcGeoidIUnit;              // 51
    float fAgeDGPS;                 // 55
    uint16_t sRefStationId;         // 57
};

class RDINMEAGGAString : public RDIPD0Interface
{
public:
    RDINMEAGGAString();
    RDINMEAGGAString(const RDINMEAGGAString& msg);
    virtual ~RDINMEAGGAString();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return IdCode; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
    
    static const int SIZE_BYTES = 98;        
    
    void set(const char* GGAString);
    
    const uint16_t IdCode = Id;
    
    // For now do this:
    char GGAString[84];
};

class RDINMEAVTGBinary : public RDIPD0Interface
{
public:
    
    RDINMEAVTGBinary();
    RDINMEAVTGBinary(const RDINMEAVTGBinary& msg);
    virtual ~RDINMEAVTGBinary();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return SID; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
    
    static const uint16_t SIZE_BYTES = 42;        
    static const int      SZ_HEADER_BYTES = 7;

    static const uint16_t SID = 105;
    
    const uint16_t IdCode = Id;
    char szHeader[SZ_HEADER_BYTES];
    float fCOGTrue;
    char tcTrueIndicator;
    float fCOGMag;
    char tcMagnIndicator;
    float fSpdOverGroundKts;
    char tcKtsIndicator;
    float fSpdOverGroundKmh;
    char tcKmhIndicator;
    char tcModeIndicator;
};

class RDINMEAVTGString : public RDIPD0Interface
{
public:
    RDINMEAVTGString();
    RDINMEAVTGString(const RDINMEAVTGString& msg);
    virtual ~RDINMEAVTGString();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return IdCode; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
    
    static const int SIZE_BYTES = 60;        
    
    void set(const char* VTGString);
    
    const uint16_t IdCode = Id;
    
    // For now do this:
    char VTGString[46];
};

class RDINMEADBTBinary : public RDIPD0Interface
{
public:
    
    RDINMEADBTBinary();
    RDINMEADBTBinary(const RDINMEADBTBinary& msg);
    virtual ~RDINMEADBTBinary();
    
    static const uint16_t Id;    
    
    uint16_t get_ID() const { return SID; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
   
    static const uint16_t SIZE_BYTES = 36;        
    static const int      SZ_HEADER_BYTES = 7;

    static const uint16_t SID = 106;
    
    const uint16_t IdCode = Id;
    char szHeader[SZ_HEADER_BYTES];
    float fWaterDepth_ft;
    char tcFeetIndicator;
    float fWaterDepth_m;
    char tcMeterIndicator;
    float fWaterDepth_F;
    char tcFathomIndicator;
};

class RDINMEADBTString : public RDIPD0Interface
{
public:
    RDINMEADBTString();
    RDINMEADBTString(const RDINMEADBTString& msg);
    virtual ~RDINMEADBTString();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return IdCode; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
    
    static const int SIZE_BYTES = 44;        
    
    void set(const char* DBTString);
    
    const uint16_t IdCode = Id;
    
    // For now do this:
    char DBTString[30];
};

class RDINMEAHDTBinary : public RDIPD0Interface
{
public:
    
    RDINMEAHDTBinary();
    RDINMEAHDTBinary(const RDINMEAHDTBinary& msg);
    virtual ~RDINMEAHDTBinary();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return SID; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
    
    static const uint16_t SIZE_BYTES = 30;        
    static const int      SZ_HEADER_BYTES = 7;

    void set(char (&szHeader)[SZ_HEADER_BYTES],
             double& dHeading,
             char& tcHeadingIndicator);
    
    static const uint16_t SID = 107;
    
    const uint16_t IdCode = Id;
    char szHeader[SZ_HEADER_BYTES];
    double dHeading;
    char tcHeadingIndicator;
};

class RDINMEAHDTString : public RDIPD0Interface
{
public:
    RDINMEAHDTString();
    RDINMEAHDTString(const RDINMEAHDTString& msg);
    virtual ~RDINMEAHDTString();
    
    static const int MESSAGE_INDEX = 15;

    static const uint16_t Id;    

    uint16_t get_ID() const { return IdCode; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
    
    static const int SIZE_BYTES = 32;        
    
    void set(const char* HDTString);
    
    const uint16_t IdCode = Id;
    
    // For now do this:
    char HDTString[18];
};

class SONSystemConfig : public RDIPD0Interface
{
public:
    SONSystemConfig();
    SONSystemConfig(const SONSystemConfig& msg);
    virtual ~SONSystemConfig();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return IdCode; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
    
    static const int SIZE_BYTES = 50;        
    static const int SERIAL_NUMBER_BYTES = 3;

    const uint16_t IdCode = Id;
    uint16_t TypeNumber;
    uint16_t SubTypeField;
    uint8_t SerialNumber[SERIAL_NUMBER_BYTES];
    uint8_t SequenceNumber;
    uint32_t DSPFirmwareVersion;
    uint32_t ARMFirmwareVersion;
    uint32_t FPGAFirmwareVersion;
    uint32_t WebServerFirmwareVersion;
    float OffsetXGPS;
    float OffsetYGPS;
    float OffsetZGPS;
    float PitchOffsetDegrees;
    float RollOffsetDegrees;
    float HeadingOffsetDegrees;
};

class SONVariableLeader : public RDIPD0Interface
{
public:
    SONVariableLeader();
    SONVariableLeader(const SONVariableLeader& msg);
    virtual ~SONVariableLeader();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return IdCode; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
    
    static const int SIZE_BYTES = 38;        
    static const int T0TIME_BYTES = 6;

    const uint16_t IdCode = Id;
    uint8_t Status;
    uint8_t PingId;
    uint8_t T0Time[T0TIME_BYTES];
    uint32_t Waveform;
    uint16_t RxGain;
    uint16_t TxGain;
    uint8_t SensorValidity;
    int16_t INSBeam0VelMms;
    int16_t INSBeam1VelMms;
    int16_t INSBeam2VelMms;
    int16_t INSBeam3VelMms;
    uint16_t INSHeading;
    int16_t  INSPitch;
    int16_t  INSRoll;
    uint16_t BeamAmbLimitMms;
    uint8_t  BeamVelRmsMms;
    uint16_t Reserved;
};

class SONOriginSystemLeader : public RDIPD0Interface
{
public:
    SONOriginSystemLeader();
    SONOriginSystemLeader(const SONOriginSystemLeader& msg);
    virtual ~SONOriginSystemLeader();

    static const uint16_t Id;    

    uint16_t get_ID() const { return IdCode; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
        
    static const int SIZE_BYTES = 72;        

    const uint16_t IdCode = Id;
	uint32_t u32_SerialNum;
	uint16_t u16_SubSerialNum;
	uint16_t u16_TypeNum;
	uint16_t u16_SubTypeNum;
	uint16_t u16_BeamControl;
	uint16_t u16_EventID;
	uint16_t u16_PingID;
	uint16_t u16_BurstID;
	uint16_t u16_CaptureDurationS;
	uint32_t u32_SleepDurationS;
	uint16_t u16_PingFrequency_mHz;
	uint16_t u16_BeamIndex;
	uint32_t u32_PingCounter;
	uint16_t u16_NoBeams;
	uint32_t u32_EventStartTimeS;
	uint32_t u32_EventStartTime_us;
	uint64_t u64_UnixEpochTime_ms;
	uint32_t u32_CarrierFrequencyHz;
	uint32_t u32_RxBandwidthHz;
	uint32_t u32_TxBandwidthHz;
	uint16_t u16_RxGaindB;
	uint16_t u16_TxLevel;
	uint16_t u16_SIG_BaseCounts;
	uint16_t u16_SIG_BT;
};

class SONPIESVariableLeader : public RDIPD0Interface
{
public:
    SONPIESVariableLeader();
    SONPIESVariableLeader(const SONPIESVariableLeader& msg);
    virtual ~SONPIESVariableLeader();
    
    static const uint16_t Id;    

    uint16_t get_ID() const { return IdCode; }
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
            
    static const int SIZE_BYTES = 74;

    const uint16_t IdCode = Id;
    float    SV1_ms;  // 4
    float    SV2_ms;  // 4 8
    float    TOF1_s; // 4 12
    float    TOF2_s; // 4 16
    float    Magnitude;  // 4 20
    uint32_t PeakIndex; // 4 24
    uint32_t RMLEIndex; // 4 28
    float RMLERange;    // 4 32
    float PretriggerRMS; // 4 36
    float HalfLife_s;      // 4 40
    float R2;            // 4 44
    float Pressure_kPa;      // 4 48
    float Depth_m;         // 4 52
    float Zoffset_m;       // 4 56
    float Latitude_DD;      // 4 60
    float Patmos_kPa;        // 4 64
    uint64_t TimeNS;     // 8 72
    
private:

};

class PD0Message
{
public:
    PD0Message();
    PD0Message(const PD0Message& orig);
    virtual ~PD0Message();

    void add_section(RDIPD0Interface* pd0_section);    
    RDIPD0Interface* get_section(const uint16_t& id) const;
    size_t calc_length() const;
    int serialise(char* buf, const int bytes) const;    
    int deserialise(const char* buf, const int bytes);    
           
    void transform(const EFrame& eFrame);
    
    uint16_t Reserved;
    uint16_t Checksum;

    static const int RESERVED_SIZE_BYTES = sizeof(uint16_t);
    static const int CHECKSUM_SIZE_BYTES = sizeof(uint16_t);

private:

    bool section_present(const uint16_t& Id);        
    uint16_t calc_checksum(const char* buf, const int& buf_size_bytes) const;
    std::vector<uint16_t> calc_indices() const;

    typedef std::map<uint16_t, RDIPD0Interface*>::iterator fields_it_t;
    typedef std::map<uint16_t, RDIPD0Interface*>::const_iterator fields_cit_t;
    std::map<uint16_t, RDIPD0Interface*> ma_Fields;
};

}

}

#endif /* PDN_MESSAGES_H */

