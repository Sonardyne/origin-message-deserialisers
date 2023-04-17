#ifndef ADCP_COREDATA_MESSAGES_H
/// \exclude
#define ADCP_COREDATA_MESSAGES_H

#include <vector>
#include <stdint.h>
#include <string.h>

namespace sonardyne_edge
{

namespace adcp_coredata_messages
{

static const uint8_t PING_PROCESSOR_MAXBEAMS = 5;
static const uint16_t ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES = 5000;
static const uint16_t ADCP55_SENSOR_DGRAM_MAXDATA_SAMPLES = 2500;
static const uint16_t ADCP55_SENSOR_AGRAM_MAX_PROCESSEDBINS = 2500;


enum ETxGainLevel
{
	eTX_LOW = 0,
	eTX_MED = 1,
	eTX_HIGH = 2,
	eTX_MAX = 3
};

class CInstrumentTime
{
public:
	CInstrumentTime();
	CInstrumentTime(const CInstrumentTime& msg);
	~CInstrumentTime();

	size_t calc_length() const;
	int deserialise(const char* buf, const int bytes);

	uint32_t u32_seconds;
	uint32_t u32_microseconds;

	CInstrumentTime& operator=(const CInstrumentTime& msg)
	{
		u32_seconds = msg.u32_seconds;
		u32_microseconds = msg.u32_microseconds;
                
                return *this;
	}

	bool operator==(const CInstrumentTime& msg) const
	{
		bool equal = true;
		equal &= (u32_seconds == msg.u32_seconds);
		equal &= (u32_microseconds == msg.u32_microseconds);
		return equal;
	}

	bool operator<(const CInstrumentTime& msg) const
	{
		bool lhs_less = false;
		lhs_less |= (u32_seconds < msg.u32_seconds);
		lhs_less |= (u32_microseconds < msg.u32_microseconds);
		return lhs_less;
	}

private:
};

class SPingCommon
{
public:
	SPingCommon();
	SPingCommon(const SPingCommon& msg);
	~SPingCommon();

	size_t calc_length() const;
	int deserialise(const char* buf, const int bytes);

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
	CInstrumentTime ciT_EventStartTime;
	uint64_t u64_UnixEpochTime_ms;
	uint32_t u32_CarrierFrequencyHz;
	uint32_t u32_RxBandwidthHz;
	uint32_t u32_TxBandwidthHz;
	uint16_t u16_RxGaindB;
	uint16_t u16_TxLevel;
	uint16_t u16_SIG_BaseCounts;
	uint16_t u16_SIG_BT;
	int16_t n16_Roll_cdeg;
	int16_t n16_Pitch_cdeg;
	int32_t n32_Heading_cdeg;
	CInstrumentTime ciT_LocalSensorTimestamp;
	int16_t n16_accX_mms2;
	int16_t n16_accY_mms2;
	int16_t n16_accZ_mms2;
	uint32_t u32_PressurePa;
	int32_t n32_Temp_mdegC;
	CInstrumentTime ciT_RemoteSensorTimestamp;
        uint16_t u16_PingsPerEnsemble;
        uint16_t u16_PingErr_BndStatus;
        uint32_t u32_BndMaxXC;
	uint16_t u16_BndMaxXCpos;
	float f32_BndSnr;
	float f32_BndFine_Hz;
	float f32_BndFit_err;

	SPingCommon& operator=(const SPingCommon& msg)
	{
		u32_SerialNum = msg.u32_SerialNum;
		u16_SubSerialNum = msg.u16_SubSerialNum;
		u16_TypeNum = msg.u16_TypeNum;
		u16_SubTypeNum = msg.u16_SubTypeNum;
		u16_BeamControl = msg.u16_BeamControl;
		u16_EventID = msg.u16_EventID;
		u16_PingID = msg.u16_PingID;
		u16_BurstID = msg.u16_BurstID;
		u16_CaptureDurationS = msg.u16_CaptureDurationS;
		u32_SleepDurationS = msg.u32_SleepDurationS;
		u16_PingFrequency_mHz = msg.u16_PingFrequency_mHz;
		u16_BeamIndex = msg.u16_BeamIndex;
		u32_PingCounter = msg.u32_PingCounter;
		u16_NoBeams = msg.u16_NoBeams;
		ciT_EventStartTime = msg.ciT_EventStartTime;
		u64_UnixEpochTime_ms = msg.u64_UnixEpochTime_ms;
		u32_CarrierFrequencyHz = msg.u32_CarrierFrequencyHz;
		u32_RxBandwidthHz = msg.u32_RxBandwidthHz;
		u32_TxBandwidthHz = msg.u32_TxBandwidthHz;
		u16_RxGaindB = msg.u16_RxGaindB;
		u16_TxLevel = msg.u16_TxLevel;
		u16_SIG_BaseCounts = msg.u16_SIG_BaseCounts;
		u16_SIG_BT = msg.u16_SIG_BT;
		n16_Roll_cdeg = msg.n16_Roll_cdeg;
		n16_Pitch_cdeg = msg.n16_Pitch_cdeg;
		n32_Heading_cdeg = msg.n32_Heading_cdeg;
		ciT_LocalSensorTimestamp = msg.ciT_LocalSensorTimestamp;
		n16_accX_mms2 = msg.n16_accX_mms2;
		n16_accY_mms2 = msg.n16_accY_mms2;
		n16_accZ_mms2 = msg.n16_accZ_mms2;
		u32_PressurePa = msg.u32_PressurePa;
		n32_Temp_mdegC = msg.n32_Temp_mdegC;
		ciT_RemoteSensorTimestamp = msg.ciT_RemoteSensorTimestamp;
                u16_PingsPerEnsemble = msg.u16_PingsPerEnsemble;
                u16_PingErr_BndStatus = msg.u16_PingErr_BndStatus;
		u32_BndMaxXC = msg.u32_BndMaxXC;
		u16_BndMaxXCpos = msg.u16_BndMaxXCpos;
		f32_BndSnr = msg.f32_BndSnr;
		f32_BndFine_Hz = msg.f32_BndFine_Hz;
		f32_BndFit_err = msg.f32_BndFit_err;
		return *this;
	}

	bool operator==(const SPingCommon& msg) const
	{
		bool equal = true;
		equal &= (u32_SerialNum == msg.u32_SerialNum);
		equal &= (u16_SubSerialNum == msg.u16_SubSerialNum);
		equal &= (u16_TypeNum == msg.u16_TypeNum);
		equal &= (u16_SubTypeNum == msg.u16_SubTypeNum);
		equal &= (u16_BeamControl == msg.u16_BeamControl);
		equal &= (u16_EventID == msg.u16_EventID);
		equal &= (u16_PingID == msg.u16_PingID);
		equal &= (u16_BurstID == msg.u16_BurstID);
		equal &= (u16_CaptureDurationS == msg.u16_CaptureDurationS);
		equal &= (u32_SleepDurationS == msg.u32_SleepDurationS);
		equal &= (u16_PingFrequency_mHz == msg.u16_PingFrequency_mHz);
		equal &= (u16_BeamIndex == msg.u16_BeamIndex);
		equal &= (u32_PingCounter == msg.u32_PingCounter);
		equal &= (u16_NoBeams == msg.u16_NoBeams);
		equal &= (ciT_EventStartTime == msg.ciT_EventStartTime);
		equal &= (u64_UnixEpochTime_ms == msg.u64_UnixEpochTime_ms);
		equal &= (u32_CarrierFrequencyHz == msg.u32_CarrierFrequencyHz);
		equal &= (u32_RxBandwidthHz == msg.u32_RxBandwidthHz);
		equal &= (u32_TxBandwidthHz == msg.u32_TxBandwidthHz);
		equal &= (u16_RxGaindB == msg.u16_RxGaindB);
		equal &= (u16_TxLevel == msg.u16_TxLevel);
		equal &= (u16_SIG_BaseCounts == msg.u16_SIG_BaseCounts);
		equal &= (u16_SIG_BT == msg.u16_SIG_BT);
		equal &= (n16_Roll_cdeg == msg.n16_Roll_cdeg);
		equal &= (n16_Pitch_cdeg == msg.n16_Pitch_cdeg);
		equal &= (n32_Heading_cdeg == msg.n32_Heading_cdeg);
		equal &= (ciT_LocalSensorTimestamp == msg.ciT_LocalSensorTimestamp);
		equal &= (n16_accX_mms2 == msg.n16_accX_mms2);
		equal &= (n16_accY_mms2 == msg.n16_accY_mms2);
		equal &= (n16_accZ_mms2 == msg.n16_accZ_mms2);
		equal &= (u32_PressurePa == msg.u32_PressurePa);
		equal &= (n32_Temp_mdegC == msg.n32_Temp_mdegC);
		equal &= (ciT_RemoteSensorTimestamp == msg.ciT_RemoteSensorTimestamp);
		equal &= (u16_PingsPerEnsemble == msg.u16_PingsPerEnsemble);
		equal &= (u16_PingErr_BndStatus == msg.u16_PingErr_BndStatus);
		equal &= (u32_BndMaxXC == msg.u32_BndMaxXC);
		equal &= (u16_BndMaxXCpos == msg.u16_BndMaxXCpos);
		equal &= (f32_BndSnr == msg.f32_BndSnr);
		equal &= (f32_BndFine_Hz == msg.f32_BndFine_Hz);
		equal &= (f32_BndFit_err == msg.f32_BndFit_err);
		return equal;
	}

	bool operator<(const SPingCommon& msg) const
	{
		bool lhs_less = false;
		lhs_less |= (u32_SerialNum < msg.u32_SerialNum);
		lhs_less |= (u16_SubSerialNum < msg.u16_SubSerialNum);
		lhs_less |= (u16_TypeNum < msg.u16_TypeNum);
		lhs_less |= (u16_SubTypeNum < msg.u16_SubTypeNum);
		lhs_less |= (u16_BeamControl < msg.u16_BeamControl);
		lhs_less |= (u16_EventID < msg.u16_EventID);
		lhs_less |= (u16_PingID < msg.u16_PingID);
		lhs_less |= (u16_BurstID < msg.u16_BurstID);
		lhs_less |= (u16_CaptureDurationS < msg.u16_CaptureDurationS);
		lhs_less |= (u32_SleepDurationS < msg.u32_SleepDurationS);
		lhs_less |= (u16_PingFrequency_mHz < msg.u16_PingFrequency_mHz);
		lhs_less |= (u16_BeamIndex < msg.u16_BeamIndex);
		lhs_less |= (u32_PingCounter < msg.u32_PingCounter);
		lhs_less |= (u16_NoBeams < msg.u16_NoBeams);
		lhs_less |= (ciT_EventStartTime < msg.ciT_EventStartTime);
		lhs_less |= (u64_UnixEpochTime_ms < msg.u64_UnixEpochTime_ms);
		lhs_less |= (u32_CarrierFrequencyHz < msg.u32_CarrierFrequencyHz);
		lhs_less |= (u32_RxBandwidthHz < msg.u32_RxBandwidthHz);
		lhs_less |= (u32_TxBandwidthHz < msg.u32_TxBandwidthHz);
		lhs_less |= (u16_RxGaindB < msg.u16_RxGaindB);
		lhs_less |= (u16_TxLevel < msg.u16_TxLevel);
		lhs_less |= (u16_SIG_BaseCounts < msg.u16_SIG_BaseCounts);
		lhs_less |= (u16_SIG_BT < msg.u16_SIG_BT);
		lhs_less |= (n16_Roll_cdeg < msg.n16_Roll_cdeg);
		lhs_less |= (n16_Pitch_cdeg < msg.n16_Pitch_cdeg);
		lhs_less |= (n32_Heading_cdeg < msg.n32_Heading_cdeg);
		lhs_less |= (ciT_LocalSensorTimestamp < msg.ciT_LocalSensorTimestamp);
		lhs_less |= (n16_accX_mms2 < msg.n16_accX_mms2);
		lhs_less |= (n16_accY_mms2 < msg.n16_accY_mms2);
		lhs_less |= (n16_accZ_mms2 < msg.n16_accZ_mms2);
		lhs_less |= (u32_PressurePa < msg.u32_PressurePa);
		lhs_less |= (n32_Temp_mdegC < msg.n32_Temp_mdegC);
		lhs_less |= (ciT_RemoteSensorTimestamp < msg.ciT_RemoteSensorTimestamp);
                lhs_less |= (u16_PingsPerEnsemble < msg.u16_PingsPerEnsemble);
                lhs_less |= (u16_PingErr_BndStatus < msg.u16_PingErr_BndStatus);
		lhs_less |= (u32_BndMaxXC < msg.u32_BndMaxXC);
		lhs_less |= (u16_BndMaxXCpos < msg.u16_BndMaxXCpos);
		lhs_less |= (f32_BndSnr < msg.f32_BndSnr);
		lhs_less |= (f32_BndFine_Hz < msg.f32_BndFine_Hz);
		lhs_less |= (f32_BndFit_err < msg.f32_BndFit_err);
		return lhs_less;
	}

private:
};

class SDSP_v1_0_ADCPGram
{
public:
	SDSP_v1_0_ADCPGram();
	SDSP_v1_0_ADCPGram(const SDSP_v1_0_ADCPGram& msg);
	~SDSP_v1_0_ADCPGram();

	size_t calc_length() const;
	int deserialise(const char* buf, const int bytes);

	SPingCommon sCommon;
	uint16_t u16_XCStartSample;
	uint16_t u16_XCEndSample;
	uint16_t u16_Stride;
	int16_t an16_DPhases[ADCP55_SENSOR_AGRAM_MAX_PROCESSEDBINS];
	uint16_t au16_DPhasesErr[ADCP55_SENSOR_AGRAM_MAX_PROCESSEDBINS];
	uint16_t au16_AVIntensityMag[ADCP55_SENSOR_AGRAM_MAX_PROCESSEDBINS];
	uint16_t au16_AVXCMag[ADCP55_SENSOR_AGRAM_MAX_PROCESSEDBINS];

	SDSP_v1_0_ADCPGram& operator=(const SDSP_v1_0_ADCPGram& msg)
	{
		sCommon = msg.sCommon;
		u16_XCStartSample = msg.u16_XCStartSample;
		u16_XCEndSample = msg.u16_XCEndSample;
		u16_Stride = msg.u16_Stride;
		memcpy(&an16_DPhases[0], &msg.an16_DPhases[0], sizeof(an16_DPhases));
		memcpy(&au16_DPhasesErr[0], &msg.au16_DPhasesErr[0], sizeof(au16_DPhasesErr));
		memcpy(&au16_AVIntensityMag[0], &msg.au16_AVIntensityMag[0], sizeof(au16_AVIntensityMag));
		memcpy(&au16_AVXCMag[0], &msg.au16_AVXCMag[0], sizeof(au16_AVXCMag));
		return *this;
	}

	bool operator==(const SDSP_v1_0_ADCPGram& msg) const
	{
		bool equal = true;
		equal &= (sCommon == msg.sCommon);
		equal &= (u16_XCStartSample == msg.u16_XCStartSample);
		equal &= (u16_XCEndSample == msg.u16_XCEndSample);
		equal &= (u16_Stride == msg.u16_Stride);
		return equal;
	}

	bool operator<(const SDSP_v1_0_ADCPGram& msg) const
	{
		bool lhs_less = false;
		lhs_less |= (sCommon < msg.sCommon);
		lhs_less |= (u16_XCStartSample < msg.u16_XCStartSample);
		lhs_less |= (u16_XCEndSample < msg.u16_XCEndSample);
		lhs_less |= (u16_Stride < msg.u16_Stride);
		return lhs_less;
	}

private:
};

class SDSP_v1_0_BDGram
{
public:
	SDSP_v1_0_BDGram();
	SDSP_v1_0_BDGram(const SDSP_v1_0_BDGram& msg);
	~SDSP_v1_0_BDGram();

	size_t calc_length() const;
	int deserialise(const char* buf, const int bytes);

	enum EProcessingStatus
	{
		eProcessingNone = 0,
		eProcessingDSP = 1
	};

	SPingCommon sCommon;
	EProcessingStatus eProcessingStatus;
	uint16_t u16_SampleBitDepth;
	uint16_t u16_StartBin;
	uint16_t u16_Samples;
	int16_t an16_IPingData[ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES];
	int16_t an16_QPingData[ADCP55_SENSOR_ADCP_MAXDATA_SAMPLES];

	SDSP_v1_0_BDGram& operator=(const SDSP_v1_0_BDGram& msg)
	{
		sCommon = msg.sCommon;
		eProcessingStatus = msg.eProcessingStatus;
		u16_SampleBitDepth = msg.u16_SampleBitDepth;
		u16_StartBin = msg.u16_StartBin;
		u16_Samples = msg.u16_Samples;
		memcpy(&an16_IPingData[0], &msg.an16_IPingData[0], sizeof(an16_IPingData));
		memcpy(&an16_QPingData[0], &msg.an16_QPingData[0], sizeof(an16_QPingData));
		return *this;
	}

	bool operator==(const SDSP_v1_0_BDGram& msg) const
	{
		bool equal = true;
		equal &= (sCommon == msg.sCommon);
		equal &= (eProcessingStatus == msg.eProcessingStatus);
		equal &= (u16_SampleBitDepth == msg.u16_SampleBitDepth);
		equal &= (u16_StartBin == msg.u16_StartBin);
		equal &= (u16_Samples == msg.u16_Samples);
		return equal;
	}

	bool operator<(const SDSP_v1_0_BDGram& msg) const
	{
		bool lhs_less = false;
		lhs_less |= (sCommon < msg.sCommon);
		lhs_less |= (eProcessingStatus < msg.eProcessingStatus);
		lhs_less |= (u16_SampleBitDepth < msg.u16_SampleBitDepth);
		lhs_less |= (u16_StartBin < msg.u16_StartBin);
		lhs_less |= (u16_Samples < msg.u16_Samples);
		return lhs_less;
	}

private:
};

class SDSP_PIESGram
{
public:
	SDSP_PIESGram();
	SDSP_PIESGram(const SDSP_PIESGram& msg);
	~SDSP_PIESGram();

	size_t calc_length() const;
	int deserialise(const char* buf, const int bytes);

	CInstrumentTime ciT_PIESRequestTime;
	float f32_SV1;
	float f32_SV2;
	float f32_TOF1;
	float f32_TOF2;
	float f32_Magnitude;
	uint32_t u32_PeakIndex;
	uint32_t u32_RMLEIndex;
	float f32_RMLERange;
	float f32_PretriggerRMS;
	float f32_HalfLife;
	float f32_R2;
	float f32_Pressure;
	float f32_Depth;
	float f32_Zoffset;
	float f32_Latitude;
	float f32_Patmos;

	SDSP_PIESGram& operator=(const SDSP_PIESGram& msg)
	{
		ciT_PIESRequestTime = msg.ciT_PIESRequestTime;
		f32_SV1 = msg.f32_SV1;
		f32_SV2 = msg.f32_SV2;
		f32_TOF1 = msg.f32_TOF1;
		f32_TOF2 = msg.f32_TOF2;
		f32_Magnitude = msg.f32_Magnitude;
		u32_PeakIndex = msg.u32_PeakIndex;
		u32_RMLEIndex = msg.u32_RMLEIndex;
		f32_RMLERange = msg.f32_RMLERange;
		f32_PretriggerRMS = msg.f32_PretriggerRMS;
		f32_HalfLife = msg.f32_HalfLife;
		f32_R2 = msg.f32_R2;
		f32_Pressure = msg.f32_Pressure;
		f32_Depth = msg.f32_Depth;
		f32_Zoffset = msg.f32_Zoffset;
		f32_Latitude = msg.f32_Latitude;
		f32_Patmos = msg.f32_Patmos;
		return *this;
	}

	bool operator==(const SDSP_PIESGram& msg) const
	{
		bool equal = true;
		equal &= (ciT_PIESRequestTime == msg.ciT_PIESRequestTime);
		equal &= (f32_SV1 == msg.f32_SV1);
		equal &= (f32_SV2 == msg.f32_SV2);
		equal &= (f32_TOF1 == msg.f32_TOF1);
		equal &= (f32_TOF2 == msg.f32_TOF2);
		equal &= (f32_Magnitude == msg.f32_Magnitude);
		equal &= (u32_PeakIndex == msg.u32_PeakIndex);
		equal &= (u32_RMLEIndex == msg.u32_RMLEIndex);
		equal &= (f32_RMLERange == msg.f32_RMLERange);
		equal &= (f32_PretriggerRMS == msg.f32_PretriggerRMS);
		equal &= (f32_HalfLife == msg.f32_HalfLife);
		equal &= (f32_R2 == msg.f32_R2);
		equal &= (f32_Pressure == msg.f32_Pressure);
		equal &= (f32_Depth == msg.f32_Depth);
		equal &= (f32_Zoffset == msg.f32_Zoffset);
		equal &= (f32_Latitude == msg.f32_Latitude);
		equal &= (f32_Patmos == msg.f32_Patmos);
		return equal;
	}

	bool operator<(const SDSP_PIESGram& msg) const
	{
		bool lhs_less = false;
		lhs_less |= (ciT_PIESRequestTime < msg.ciT_PIESRequestTime);
		lhs_less |= (f32_SV1 < msg.f32_SV1);
		lhs_less |= (f32_SV2 < msg.f32_SV2);
		lhs_less |= (f32_TOF1 < msg.f32_TOF1);
		lhs_less |= (f32_TOF2 < msg.f32_TOF2);
		lhs_less |= (f32_Magnitude < msg.f32_Magnitude);
		lhs_less |= (u32_PeakIndex < msg.u32_PeakIndex);
		lhs_less |= (u32_RMLEIndex < msg.u32_RMLEIndex);
		lhs_less |= (f32_RMLERange < msg.f32_RMLERange);
		lhs_less |= (f32_PretriggerRMS < msg.f32_PretriggerRMS);
		lhs_less |= (f32_HalfLife < msg.f32_HalfLife);
		lhs_less |= (f32_R2 < msg.f32_R2);
		lhs_less |= (f32_Pressure < msg.f32_Pressure);
		lhs_less |= (f32_Depth < msg.f32_Depth);
		lhs_less |= (f32_Zoffset < msg.f32_Zoffset);
		lhs_less |= (f32_Latitude < msg.f32_Latitude);
		lhs_less |= (f32_Patmos < msg.f32_Patmos);
		return lhs_less;
	}

private:
};

}

}
#endif
