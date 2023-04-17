#include <stdio.h>
#include <string.h>
#include "adcp_coredata_messages.h"

using namespace sonardyne_edge::adcp_coredata_messages;

CInstrumentTime::CInstrumentTime() : 
	u32_seconds(0),
	u32_microseconds(0)
{
}

CInstrumentTime::CInstrumentTime(const CInstrumentTime& msg) : 
	u32_seconds(msg.u32_seconds),
	u32_microseconds(msg.u32_microseconds)
{
}

CInstrumentTime::~CInstrumentTime() 
{
}

size_t CInstrumentTime::calc_length() const
{
	size_t len = 0;

	len += sizeof(u32_seconds);
	len += sizeof(u32_microseconds);

	return len;
}

int CInstrumentTime::deserialise(const char* buf, const int bytes)
{
        int offset = 0;

        memcpy(&u32_seconds, buf + offset, sizeof(u32_seconds));
        offset += sizeof(u32_seconds);
        
        memcpy(&u32_microseconds, buf + offset, sizeof(u32_seconds));
        offset += sizeof(u32_seconds);
	
        return (offset > bytes ? -1 : offset);
}

SPingCommon::SPingCommon() : 
	u32_SerialNum(0),
	u16_SubSerialNum(0),
	u16_TypeNum(0),
	u16_SubTypeNum(0),
	u16_BeamControl(0),
	u16_EventID(0),
	u16_PingID(0),
	u16_BurstID(0),
	u16_CaptureDurationS(0),
	u32_SleepDurationS(0),
	u16_PingFrequency_mHz(0),
	u16_BeamIndex(0),
	u32_PingCounter(0),
	u16_NoBeams(0),
	ciT_EventStartTime(),
	u64_UnixEpochTime_ms(0),
	u32_CarrierFrequencyHz(0),
	u32_RxBandwidthHz(0),
	u32_TxBandwidthHz(0),
	u16_RxGaindB(0),
	u16_TxLevel(0),
	u16_SIG_BaseCounts(0),
	u16_SIG_BT(0),
	n16_Roll_cdeg(0),
	n16_Pitch_cdeg(0),
	n32_Heading_cdeg(0),
	ciT_LocalSensorTimestamp(),
	n16_accX_mms2(0),
	n16_accY_mms2(0),
	n16_accZ_mms2(0),
	u32_PressurePa(0),
	n32_Temp_mdegC(0),
	ciT_RemoteSensorTimestamp(),
        u16_PingsPerEnsemble(0),
        u16_PingErr_BndStatus(0),
	u32_BndMaxXC(0),
	u16_BndMaxXCpos(0),
	f32_BndSnr(0),
	f32_BndFine_Hz(0),
	f32_BndFit_err(0)
{
}

SPingCommon::SPingCommon(const SPingCommon& msg) : 
	u32_SerialNum(msg.u32_SerialNum),
	u16_SubSerialNum(msg.u16_SubSerialNum),
	u16_TypeNum(msg.u16_TypeNum),
	u16_SubTypeNum(msg.u16_SubTypeNum),
	u16_BeamControl(msg.u16_BeamControl),
	u16_EventID(msg.u16_EventID),
	u16_PingID(msg.u16_PingID),
	u16_BurstID(msg.u16_BurstID),
	u16_CaptureDurationS(msg.u16_CaptureDurationS),
	u32_SleepDurationS(msg.u32_SleepDurationS),
	u16_PingFrequency_mHz(msg.u16_PingFrequency_mHz),
	u16_BeamIndex(msg.u16_BeamIndex),
	u32_PingCounter(msg.u32_PingCounter),
	u16_NoBeams(msg.u16_NoBeams),
	ciT_EventStartTime(msg.ciT_EventStartTime),
	u64_UnixEpochTime_ms(msg.u64_UnixEpochTime_ms),
	u32_CarrierFrequencyHz(msg.u32_CarrierFrequencyHz),
	u32_RxBandwidthHz(msg.u32_RxBandwidthHz),
	u32_TxBandwidthHz(msg.u32_TxBandwidthHz),
	u16_RxGaindB(msg.u16_RxGaindB),
	u16_TxLevel(msg.u16_TxLevel),
	u16_SIG_BaseCounts(msg.u16_SIG_BaseCounts),
	u16_SIG_BT(msg.u16_SIG_BT),
	n16_Roll_cdeg(msg.n16_Roll_cdeg),
	n16_Pitch_cdeg(msg.n16_Pitch_cdeg),
	n32_Heading_cdeg(msg.n32_Heading_cdeg),
	ciT_LocalSensorTimestamp(msg.ciT_LocalSensorTimestamp),
	n16_accX_mms2(msg.n16_accX_mms2),
	n16_accY_mms2(msg.n16_accY_mms2),
	n16_accZ_mms2(msg.n16_accZ_mms2),
	u32_PressurePa(msg.u32_PressurePa),
	n32_Temp_mdegC(msg.n32_Temp_mdegC),
	ciT_RemoteSensorTimestamp(msg.ciT_RemoteSensorTimestamp),
        u16_PingsPerEnsemble(msg.u16_PingsPerEnsemble),
        u16_PingErr_BndStatus(msg.u16_PingErr_BndStatus),
	u32_BndMaxXC(msg.u32_BndMaxXC),
	u16_BndMaxXCpos(msg.u16_BndMaxXCpos),
	f32_BndSnr(msg.f32_BndSnr),
	f32_BndFine_Hz(msg.f32_BndFine_Hz),
	f32_BndFit_err(msg.f32_BndFit_err)
{
}

SPingCommon::~SPingCommon() 
{
}

size_t SPingCommon::calc_length() const
{
	size_t len = 0;

	len += sizeof(u32_SerialNum);
	len += sizeof(u16_SubSerialNum);
	len += sizeof(u16_TypeNum);		
	len += sizeof(u16_SubTypeNum);
	len += sizeof(u16_BeamControl);	
	len += sizeof(u16_EventID);	
	len += sizeof(u16_PingID);
	len += sizeof(u16_BurstID);
	len += sizeof(u16_CaptureDurationS);
	len += sizeof(u32_SleepDurationS);
	len += sizeof(u16_PingFrequency_mHz);			
	len += sizeof(u16_BeamIndex);
	len += sizeof(u32_PingCounter);
	len += sizeof(u16_NoBeams);
	len += ciT_EventStartTime.calc_length();
	len += sizeof(u64_UnixEpochTime_ms);
	len += sizeof(u32_CarrierFrequencyHz);
	len += sizeof(u32_RxBandwidthHz);
	len += sizeof(u32_TxBandwidthHz);
	len += sizeof(u16_RxGaindB);
	len += sizeof(u16_TxLevel);
	len += sizeof(u16_SIG_BaseCounts);
	len += sizeof(u16_SIG_BT);
	len += sizeof(n16_Roll_cdeg);
	len += sizeof(n16_Pitch_cdeg);
	len += sizeof(n32_Heading_cdeg);
	len += ciT_LocalSensorTimestamp.calc_length();
	len += sizeof(n16_accX_mms2);
	len += sizeof(n16_accY_mms2);
	len += sizeof(n16_accZ_mms2);
	len += sizeof(u32_PressurePa);
	len += sizeof(n32_Temp_mdegC);
	len += ciT_RemoteSensorTimestamp.calc_length();
        len += sizeof(u16_PingsPerEnsemble);
	len += sizeof(u16_PingErr_BndStatus);
	len += sizeof(u32_BndMaxXC);
	len += sizeof(u16_BndMaxXCpos);
	len += sizeof(f32_BndSnr);
	len += sizeof(f32_BndFine_Hz);
	len += sizeof(f32_BndFit_err);

	return len;
}

int SPingCommon::deserialise(const char* buf, const int bytes)
{
    int offset = 0;
    
    memcpy(&u32_SerialNum, buf + offset, sizeof(u32_SerialNum));
    offset += sizeof(u32_SerialNum);
    
    memcpy(&u16_SubSerialNum, buf + offset, sizeof(u16_SubSerialNum));
    offset += sizeof(u16_SubSerialNum);
    
    memcpy(&u16_TypeNum, buf + offset, sizeof(u16_TypeNum));
    offset += sizeof(u16_TypeNum);

    memcpy(&u16_SubTypeNum, buf + offset, sizeof(u16_SubTypeNum));
    offset += sizeof(u16_SubTypeNum);        

    memcpy(&u16_BeamControl, buf + offset, sizeof(u16_BeamControl));
    offset += sizeof(u16_BeamControl);  
            
    memcpy(&u16_EventID, buf + offset, sizeof(u16_EventID));
    offset += sizeof(u16_EventID);
    
    memcpy(&u16_PingID, buf + offset, sizeof(u16_PingID));
    offset += sizeof(u16_PingID);
    
    memcpy(&u16_BurstID, buf + offset, sizeof(u16_BurstID));
    offset += sizeof(u16_BurstID);

    memcpy(&u16_CaptureDurationS, buf + offset, sizeof(u16_CaptureDurationS));
    offset += sizeof(u16_CaptureDurationS);
    
    memcpy(&u32_SleepDurationS, buf + offset, sizeof(u32_SleepDurationS));
    offset += sizeof(u32_SleepDurationS);
    
    memcpy(&u16_PingFrequency_mHz, buf + offset, sizeof(u16_PingFrequency_mHz));
    offset += sizeof(u16_PingFrequency_mHz);        
    
    memcpy(&u16_BeamIndex, buf + offset, sizeof(u16_BeamIndex));
    offset += sizeof(u16_BeamIndex);
    
    memcpy(&u32_PingCounter, buf + offset, sizeof(u32_PingCounter));
    offset += sizeof(u32_PingCounter);
    
    memcpy(&u16_NoBeams, buf + offset, sizeof(u16_NoBeams));
    offset += sizeof(u16_NoBeams);
    
    offset += ciT_EventStartTime.deserialise(buf + offset, bytes - offset);
    
    memcpy(&u64_UnixEpochTime_ms, buf + offset, sizeof(u64_UnixEpochTime_ms));
    offset += sizeof(u64_UnixEpochTime_ms);
    
    memcpy(&u32_CarrierFrequencyHz, buf + offset, sizeof(u32_CarrierFrequencyHz));
    offset += sizeof(u32_CarrierFrequencyHz);
    
    memcpy(&u32_RxBandwidthHz, buf + offset, sizeof(u32_RxBandwidthHz));
    offset += sizeof(u32_RxBandwidthHz);
    
    memcpy(&u32_TxBandwidthHz, buf + offset, sizeof(u32_TxBandwidthHz));
    offset += sizeof(u32_TxBandwidthHz);
    
    memcpy(&u16_RxGaindB, buf + offset, sizeof(u16_RxGaindB));
    offset += sizeof(u16_RxGaindB);
    
    memcpy(&u16_TxLevel, buf + offset, sizeof(u16_TxLevel));
    offset += sizeof(u16_TxLevel);
    
    memcpy(&u16_SIG_BaseCounts, buf + offset, sizeof(u16_SIG_BaseCounts));
    offset += sizeof(u16_SIG_BaseCounts);
    
    memcpy(&u16_SIG_BT, buf + offset, sizeof(u16_SIG_BT));
    offset += sizeof(u16_SIG_BT);

    memcpy(&n16_Roll_cdeg, buf + offset, sizeof(n16_Roll_cdeg));
    offset += sizeof(n16_Roll_cdeg);
    
    memcpy(&n16_Pitch_cdeg, buf + offset, sizeof(n16_Pitch_cdeg));
    offset += sizeof(n16_Pitch_cdeg);
    
    memcpy(&n32_Heading_cdeg, buf + offset, sizeof(n32_Heading_cdeg));
    offset += sizeof(n32_Heading_cdeg);
    
    offset += ciT_LocalSensorTimestamp.deserialise(buf + offset, bytes - offset);
    
    memcpy(&n16_accX_mms2, buf + offset, sizeof(n16_accX_mms2));
    offset += sizeof(n16_accX_mms2);
    
    memcpy(&n16_accY_mms2, buf + offset, sizeof(n16_accY_mms2));
    offset += sizeof(n16_accY_mms2);
    
    memcpy(&n16_accZ_mms2, buf + offset, sizeof(n16_accZ_mms2));
    offset += sizeof(n16_accZ_mms2);
    
    memcpy(&u32_PressurePa, buf + offset, sizeof(u32_PressurePa));
    offset += sizeof(u32_PressurePa);
    
    memcpy(&n32_Temp_mdegC, buf + offset, sizeof(n32_Temp_mdegC));
    offset += sizeof(n32_Temp_mdegC);
    
    offset += ciT_RemoteSensorTimestamp.deserialise(buf + offset, bytes - offset);

    memcpy(&u16_PingsPerEnsemble, buf + offset, sizeof(u16_PingsPerEnsemble));
    offset += sizeof(u16_PingsPerEnsemble);
    
    memcpy(&u16_PingErr_BndStatus, buf + offset, sizeof(u16_PingErr_BndStatus));
    offset += sizeof(u16_PingErr_BndStatus);	
    
    memcpy(&u32_BndMaxXC, buf + offset, sizeof(u32_BndMaxXC));
    offset += sizeof(u32_BndMaxXC);

    memcpy(&u16_BndMaxXCpos, buf + offset, sizeof(u16_BndMaxXCpos));
    offset += sizeof(u16_BndMaxXCpos);

    memcpy(&f32_BndSnr, buf + offset, sizeof(f32_BndSnr));
    offset += sizeof(f32_BndSnr);

    memcpy(&f32_BndFine_Hz, buf + offset, sizeof(f32_BndFine_Hz));
    offset += sizeof(f32_BndFine_Hz);

    memcpy(&f32_BndFit_err, buf + offset, sizeof(f32_BndFit_err));
    offset += sizeof(f32_BndFit_err);

    return offset;
}

SDSP_v1_0_ADCPGram::SDSP_v1_0_ADCPGram() :
	sCommon(),
	u16_XCStartSample(0),
	u16_XCEndSample(0),
	u16_Stride(0)
{
	memset(&an16_DPhases[0], 0x0, sizeof(an16_DPhases));
	memset(&au16_DPhasesErr[0], 0x0, sizeof(au16_DPhasesErr));
	memset(&au16_AVIntensityMag[0], 0x0, sizeof(au16_AVIntensityMag));
	memset(&au16_AVXCMag[0], 0x0, sizeof(au16_AVXCMag));
}

SDSP_v1_0_ADCPGram::SDSP_v1_0_ADCPGram(const SDSP_v1_0_ADCPGram& msg) :
	sCommon(msg.sCommon),
	u16_XCStartSample(msg.u16_XCStartSample),
	u16_XCEndSample(msg.u16_XCEndSample),
	u16_Stride(msg.u16_Stride)
{
	memcpy(&an16_DPhases[0], &msg.an16_DPhases[0], sizeof(an16_DPhases));
	memcpy(&au16_DPhasesErr[0], &msg.au16_DPhasesErr[0], sizeof(au16_DPhasesErr));
	memcpy(&au16_AVIntensityMag[0], &msg.au16_AVIntensityMag[0], sizeof(au16_AVIntensityMag));
	memcpy(&au16_AVXCMag[0], &msg.au16_AVXCMag[0], sizeof(au16_AVXCMag));
}

SDSP_v1_0_ADCPGram::~SDSP_v1_0_ADCPGram()
{

}

size_t SDSP_v1_0_ADCPGram::calc_length() const
{
	size_t len = 0;
	len += sCommon.calc_length();

	len += sizeof(u16_XCStartSample);
	len += sizeof(u16_XCEndSample);
	len += sizeof(u16_Stride);
	len += sizeof(an16_DPhases);
	len += sizeof(au16_DPhasesErr);
	len += sizeof(au16_AVIntensityMag);
	len += sizeof(au16_AVXCMag);

	return len;
}

int SDSP_v1_0_ADCPGram::deserialise(const char* buf, const int bytes)
{

    int offset = 0;
    offset += sCommon.deserialise(buf + offset, bytes - offset);

    memcpy(&u16_XCStartSample, buf + offset, sizeof(u16_XCStartSample));
    offset += sizeof(u16_XCStartSample);
    
    memcpy(&u16_XCEndSample, buf + offset, sizeof(u16_XCEndSample));
    offset += sizeof(u16_XCEndSample);
    
    memcpy(&u16_Stride, buf + offset, sizeof(u16_Stride));
    offset += sizeof(u16_Stride);
    
    memcpy(&an16_DPhases[0], buf + offset, sizeof(an16_DPhases));
    offset += sizeof(an16_DPhases);
    
    memcpy(&au16_DPhasesErr[0], buf + offset, sizeof(au16_DPhasesErr));
    offset += sizeof(au16_DPhasesErr);
    
    memcpy(&au16_AVIntensityMag[0], buf + offset, sizeof(au16_AVIntensityMag));
    offset += sizeof(au16_AVIntensityMag);
    
    memcpy(&au16_AVXCMag[0], buf + offset, sizeof(au16_AVXCMag));
    offset += sizeof(au16_AVXCMag);
    
    return offset;
}

SDSP_v1_0_BDGram::SDSP_v1_0_BDGram() : 
	sCommon(),
	eProcessingStatus((EProcessingStatus) 0),
	u16_SampleBitDepth(0),
	u16_StartBin(0),
	u16_Samples(0)
{
	memset(&an16_IPingData[0], 0x0, sizeof(an16_IPingData));
	memset(&an16_QPingData[0], 0x0, sizeof(an16_QPingData));
}

SDSP_v1_0_BDGram::SDSP_v1_0_BDGram(const SDSP_v1_0_BDGram& msg) : 
	sCommon(msg.sCommon),
	eProcessingStatus(msg.eProcessingStatus),
	u16_SampleBitDepth(msg.u16_SampleBitDepth),
	u16_StartBin(msg.u16_StartBin),
	u16_Samples(msg.u16_Samples)
{
	memcpy(&an16_IPingData[0], &msg.an16_IPingData[0], sizeof(an16_IPingData));
	memcpy(&an16_QPingData[0], &msg.an16_QPingData[0], sizeof(an16_QPingData));
}

SDSP_v1_0_BDGram::~SDSP_v1_0_BDGram() 
{
}

size_t SDSP_v1_0_BDGram::calc_length() const
{
	size_t len = 0;

	len += sCommon.calc_length();
	uint16_t tmp_eProcessingStatus = (uint16_t) eProcessingStatus;
	len += sizeof(tmp_eProcessingStatus);
	len += sizeof(u16_SampleBitDepth);
	len += sizeof(u16_StartBin);
	len += sizeof(u16_Samples);
	len += sizeof(an16_IPingData);
	len += sizeof(an16_QPingData);

	return len;
}

int SDSP_v1_0_BDGram::deserialise(const char* buf, const int bytes)
{

    int offset = 0;
    
    offset += sCommon.deserialise(buf + offset, bytes - offset);
        
    uint16_t u16_tmp = 0;
    memcpy(&u16_tmp, buf + offset, sizeof(u16_tmp));
    offset += sizeof(u16_tmp);    
    eProcessingStatus = (EProcessingStatus) u16_tmp;
    
    memcpy(&u16_SampleBitDepth, buf + offset, sizeof(u16_SampleBitDepth));
    offset += sizeof(u16_SampleBitDepth);
    
    memcpy(&u16_StartBin, buf + offset, sizeof(u16_StartBin));
    offset += sizeof(u16_StartBin);
    
    memcpy(&u16_Samples, buf + offset, sizeof(u16_Samples));
    offset += sizeof(u16_Samples);
    
    memcpy(&an16_IPingData[0], buf + offset, sizeof(an16_IPingData));
    offset += sizeof(an16_IPingData);
    
    memcpy(&an16_QPingData[0], buf + offset, sizeof(an16_QPingData));
    offset += sizeof(an16_QPingData);
    
    return offset;
}

SDSP_PIESGram::SDSP_PIESGram() : 
	ciT_PIESRequestTime(),
	f32_SV1(0),
	f32_SV2(0),
	f32_TOF1(0),
	f32_TOF2(0),
	f32_Magnitude(0),
	u32_PeakIndex(0),
	u32_RMLEIndex(0),
	f32_RMLERange(0),
	f32_PretriggerRMS(0),
	f32_HalfLife(0),
	f32_R2(0),
	f32_Pressure(0),
	f32_Depth(0),
	f32_Zoffset(0),
	f32_Latitude(0),
	f32_Patmos(0)
{
}

SDSP_PIESGram::SDSP_PIESGram(const SDSP_PIESGram& msg) : 
	ciT_PIESRequestTime(msg.ciT_PIESRequestTime),
	f32_SV1(msg.f32_SV1),
	f32_SV2(msg.f32_SV2),
	f32_TOF1(msg.f32_TOF1),
	f32_TOF2(msg.f32_TOF2),
	f32_Magnitude(msg.f32_Magnitude),
	u32_PeakIndex(msg.u32_PeakIndex),
	u32_RMLEIndex(msg.u32_RMLEIndex),
	f32_RMLERange(msg.f32_RMLERange),
	f32_PretriggerRMS(msg.f32_PretriggerRMS),
	f32_HalfLife(msg.f32_HalfLife),
	f32_R2(msg.f32_R2),
	f32_Pressure(msg.f32_Pressure),
	f32_Depth(msg.f32_Depth),
	f32_Zoffset(msg.f32_Zoffset),
	f32_Latitude(msg.f32_Latitude),
	f32_Patmos(msg.f32_Patmos)
{
}

SDSP_PIESGram::~SDSP_PIESGram() 
{
}

size_t SDSP_PIESGram::calc_length() const
{
	size_t len = 0;

	len += ciT_PIESRequestTime.calc_length();
	len += sizeof(f32_SV1),
	len += sizeof(f32_SV2);
	len += sizeof(f32_TOF1);
	len += sizeof(f32_TOF2);
	len += sizeof(f32_Magnitude);
	len += sizeof(u32_PeakIndex);
	len += sizeof(u32_RMLEIndex);
	len += sizeof(f32_RMLERange);
	len += sizeof(f32_PretriggerRMS);
	len += sizeof(f32_HalfLife);
	len += sizeof(f32_R2);
	len += sizeof(f32_Pressure);
	len += sizeof(f32_Depth);
	len += sizeof(f32_Zoffset);
	len += sizeof(f32_Latitude);
	len += sizeof(f32_Patmos);
	
	return len;
}

int SDSP_PIESGram::deserialise(const char* buf, const int bytes)
{

    int offset = 0;
    
    offset += ciT_PIESRequestTime.deserialise(buf + offset, bytes - offset);
        
    memcpy(&f32_SV1, buf + offset, sizeof(f32_SV1));
    offset += sizeof(f32_SV1);
    
    memcpy(&f32_SV2, buf + offset, sizeof(f32_SV2));
    offset += sizeof(f32_SV2);
    
    memcpy(&f32_TOF1, buf + offset, sizeof(f32_TOF1));
    offset += sizeof(f32_TOF1);
    
    memcpy(&f32_TOF2, buf + offset, sizeof(f32_TOF2));
    offset += sizeof(f32_TOF2);
    
    memcpy(&f32_Magnitude, buf + offset, sizeof(f32_Magnitude));
    offset += sizeof(f32_Magnitude);
    
    memcpy(&u32_PeakIndex, buf + offset, sizeof(u32_PeakIndex));
    offset += sizeof(u32_PeakIndex);
    
    memcpy(&u32_RMLEIndex, buf + offset, sizeof(u32_RMLEIndex));
    offset += sizeof(u32_RMLEIndex);
    
    memcpy(&f32_RMLERange, buf + offset, sizeof(f32_RMLERange));
    offset += sizeof(f32_RMLERange);
    
    memcpy(&f32_PretriggerRMS, buf + offset, sizeof(f32_PretriggerRMS));
    offset += sizeof(f32_PretriggerRMS);
    
    memcpy(&f32_HalfLife, buf + offset, sizeof(f32_HalfLife));
    offset += sizeof(f32_HalfLife);
    
    memcpy(&f32_R2, buf + offset, sizeof(f32_R2));
    offset += sizeof(f32_R2);
    
    memcpy(&f32_Pressure, buf + offset, sizeof(f32_Pressure));
    offset += sizeof(f32_Pressure);
    
    memcpy(&f32_Depth, buf + offset, sizeof(f32_Depth));
    offset += sizeof(f32_Depth);
    
    memcpy(&f32_Zoffset, buf + offset, sizeof(f32_Zoffset));
    offset += sizeof(f32_Zoffset);

    memcpy(&f32_Latitude, buf + offset, sizeof(f32_Latitude));
    offset += sizeof(f32_Latitude);

    memcpy(&f32_Patmos, buf + offset, sizeof(f32_Patmos));
    offset += sizeof(f32_Patmos);
    
    return offset;
}
