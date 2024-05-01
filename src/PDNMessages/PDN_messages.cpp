#include <unistd.h>
#include <string.h>
#include <iostream>

#include "PDN_messages.h"

using namespace sonardyne_edge::PDN_messages;

const uint16_t RDIPD0Header::Id             = 0x7F7F;
const uint16_t RDIFixedLeader::Id           = 0x0000;
const uint16_t RDIVariableLeader::Id        = 0x0080;
const uint16_t RDIVelocity::Id              = 0x0100;
const uint16_t RDIXC::Id                    = 0x0200;
const uint16_t RDIIntensity::Id             = 0x0300;
const uint16_t RDIPrctGood::Id              = 0x0400;
const uint16_t RDIVerticalBeamLeader::Id    = 0x0F01;
const uint16_t RDIVerticalBeamVelocity::Id  = 0x0A00;
const uint16_t RDIVerticalBeamXC::Id        = 0x0B00;
const uint16_t RDIVerticalBeamIntensity::Id = 0x0C00;
const uint16_t RDIVerticalBeamPrctGood::Id  = 0x0D00;
const uint16_t RDIBottomTrack::Id           = 0x0600;
const uint16_t RDINMEAGGABinary::Id         = 0x2022;
const uint16_t RDINMEAVTGBinary::Id         = 0x2022;
const uint16_t RDINMEADBTBinary::Id         = 0x2022;
const uint16_t RDINMEAHDTBinary::Id         = 0x2022;
const uint16_t RDINMEAGGAString::Id         = 0x2101;
const uint16_t RDINMEAVTGString::Id         = 0x2102;
const uint16_t RDINMEADBTString::Id         = 0x2100;
const uint16_t RDINMEAHDTString::Id         = 0x2104;
const uint16_t SONSystemConfig::Id          = 0x6060;
const uint16_t SONVariableLeader::Id        = 0x6160;
const uint16_t SONOriginSystemLeader::Id    = 0x6201;
const uint16_t SONPIESVariableLeader::Id    = 0x62CA;

const uint16_t RDINMEAGGABinary::SID    = 104;
const uint16_t RDINMEAVTGBinary::SID    = 105;

//number of beams fixed at 4 to comply with RDI standard, vertical beam is in separate section
const uint8_t RDIVelocity::_number_of_beams = 4;
const uint8_t RDIXC::_number_of_beams = 4;
const uint8_t RDIPrctGood::_number_of_beams = 4;
const uint8_t RDIIntensity::_number_of_beams = 4;

void mult3DMatrixVector(const float (&m)[3][3], int16_t (&v)[3])
{
    float w[3] = { 0 };
    
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            w[i] += m[i][j] * v[j];
        }
    }
    
    for (int i = 0; i < 3; ++i)
    {
        v[i] = w[i];
    }    
}

void rotx(const float& c, const float& s, int16_t (&v)[3])
{
    const float mx[3][3] = {{1, 0, 0},{0, c, s},{0, -s, c}};

    ::mult3DMatrixVector(mx, v);
}

void roty(const float& c, const float& s, int16_t (&v)[3])
{
    const float my[3][3] = {{c, 0, -s},{0, 1, 0},{s, 0, c}};

    ::mult3DMatrixVector(my, v);
}

void rotz(const float& c, const float& s, int16_t (&v)[3])
{
    const float mz[3][3] = {{c, s, 0},{-s, c, 0},{0, 0, 1}};

    ::mult3DMatrixVector(mz, v);
}

RDIPD0Header::RDIPD0Header() :
        HeaderId(0x7F),
        DataSource(0x7F),
        EnsembleBytes(0),
        Spare(0),
        NumberDataTypes(0)
{
}

RDIPD0Header::RDIPD0Header(const RDIPD0Header& msg) : 
        HeaderId(msg.HeaderId),
        DataSource(msg.DataSource),
        EnsembleBytes(msg.EnsembleBytes),
        Spare(msg.Spare),
        NumberDataTypes(msg.NumberDataTypes),
        Indices(msg.Indices)
{   
}

RDIPD0Header::~RDIPD0Header()
{

}

size_t RDIPD0Header::calc_length() const
{
    return (SIZE_BYTES + sizeof(uint16_t) * Indices.size());
}

int RDIPD0Header::serialise(char* buffer, const int bytes) const
{
        // PD0 serialises to binary by default
        int offset = 0;

        memcpy(buffer + offset, &HeaderId, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buffer + offset, &DataSource, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buffer + offset, &EnsembleBytes, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buffer + offset, &Spare, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buffer + offset, &NumberDataTypes, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        for (size_t k = 0; k < Indices.size(); ++k)
        {
            memcpy(buffer + offset, &Indices[k], sizeof(uint16_t));
            offset += sizeof(uint16_t);            
        }
        
        return (offset > bytes ? -1 : offset);
}

int RDIPD0Header::deserialise(const char* buf, const int bytes)
{
        int offset = 0;

        memcpy(&HeaderId, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&DataSource, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        if((HeaderId != 0x7F) || (DataSource != 0x7F))
        {
            return -1; //basic sanity check! must be valid header to deserialise
        }

        memcpy(&EnsembleBytes, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&Spare, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&NumberDataTypes, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        if (bytes == SIZE_BYTES)
        {
            return offset;
        }
        
        Indices.resize(static_cast<size_t>(NumberDataTypes));
        for (uint8_t k = 0; k < NumberDataTypes; ++k)
        {
            memcpy(&Indices[k], buf + offset, sizeof(uint16_t));
            offset += sizeof(uint16_t);            
        }
        
        return (offset > bytes ? -1 : offset);
}

void RDIPD0Header::set(uint16_t PD0EnsembleBytes, uint8_t PD0NumberDataTypes)
{
        EnsembleBytes = PD0EnsembleBytes;
        NumberDataTypes = PD0NumberDataTypes;
}

uint16_t RDIPD0Header::get_TotalEnsembleBytes()
{
        // include the checksum
        return (EnsembleBytes + 2);
}

RDIFixedLeader::RDIFixedLeader() :
        CpuFirmwareVer(0),
        CpuFirmwareRev(0),
        SystemConfig(0),
        RealOrSim(0),
        LagLengthCm(0),
        NumberOfBeams(0),
        NumberOfCells(0),
        PingsPerEnsemble(0),
        DepthCellLengthCm(0),
        BlankAfterTransmitCm(0),
        ProfilingMode(0),
        LowcorrThresh(0),
        CodeRepeats(0),
        PercentGood(0),
        ErrorVelMax(0),
        TPPMins(0),
        TPPSecs(0),
        TPPHunds(0),
        CoordTxfrm(0),
        HeadingAlignment(0),
        HeadingBias(0),
        SensorSource(0),
        SensorAvail(0),
        Bin1DistanceCm(0),
        TxPulseLengthCm(0),
        StartEndDepthCell(0),
        FalseTargetThresh(0),
        Spare1(0),
        TxLagDistanceCm(0),
        WBSystemBandwidth(0),
        SystemPower(0),
        Spare2(0),
        SerialNumber(0),
        BeamAngle(0)
{
        memset(&Spares[0], 0x0, sizeof(Spares));
}

RDIFixedLeader::RDIFixedLeader(const RDIFixedLeader& msg) :
        CpuFirmwareVer(msg.CpuFirmwareVer),
        CpuFirmwareRev(msg.CpuFirmwareRev),
        SystemConfig(msg.SystemConfig),
        RealOrSim(msg.RealOrSim),
        LagLengthCm(msg.LagLengthCm),
        NumberOfBeams(msg.NumberOfBeams),
        NumberOfCells(msg.NumberOfCells),
        PingsPerEnsemble(msg.PingsPerEnsemble),
        DepthCellLengthCm(msg.DepthCellLengthCm),
        BlankAfterTransmitCm(msg.BlankAfterTransmitCm),
        ProfilingMode(msg.ProfilingMode),
        LowcorrThresh(msg.LowcorrThresh),
        CodeRepeats(msg.CodeRepeats),
        PercentGood(msg.PercentGood),
        ErrorVelMax(msg.ErrorVelMax),
        TPPMins(msg.TPPMins),
        TPPSecs(msg.TPPSecs),
        TPPHunds(msg.TPPHunds),
        CoordTxfrm(msg.CoordTxfrm),
        HeadingAlignment(msg.HeadingAlignment),
        HeadingBias(msg.HeadingBias),
        SensorSource(msg.SensorSource),
        SensorAvail(msg.SensorAvail),
        Bin1DistanceCm(msg.Bin1DistanceCm),
        TxPulseLengthCm(msg.TxPulseLengthCm),
        StartEndDepthCell(msg.StartEndDepthCell),
        FalseTargetThresh(msg.FalseTargetThresh),
        Spare1(msg.Spare1),
        TxLagDistanceCm(msg.TxLagDistanceCm),
        WBSystemBandwidth(msg.WBSystemBandwidth),
        SystemPower(msg.SystemPower),
        Spare2(msg.Spare2),
        SerialNumber(msg.SerialNumber),
        BeamAngle(msg.BeamAngle)
{
        memcpy(&Spares[0], &msg.Spares[0], sizeof(Spares));
}

RDIFixedLeader::~RDIFixedLeader()
{

}

size_t RDIFixedLeader::calc_length() const
{
    // PD0 fields binary by default
    return SIZE_BYTES;
}

int RDIFixedLeader::serialise(char* buf, const int bytes) const
{
        int offset = 0;

        memcpy(buf + offset, &FixedLeaderID, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &CpuFirmwareVer, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &CpuFirmwareRev, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &SystemConfig, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &RealOrSim, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &LagLengthCm, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &NumberOfBeams, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &NumberOfCells, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &PingsPerEnsemble, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &DepthCellLengthCm, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &BlankAfterTransmitCm, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &ProfilingMode, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &LowcorrThresh, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &CodeRepeats, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &PercentGood, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &ErrorVelMax, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &TPPMins, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &TPPSecs, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &TPPHunds, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &CoordTxfrm, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &HeadingAlignment, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &HeadingBias, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &SensorSource, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &SensorAvail, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &Bin1DistanceCm, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &TxPulseLengthCm, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &StartEndDepthCell, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &FalseTargetThresh, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &Spare1, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &TxLagDistanceCm, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &Spares[0], 8*sizeof(uint8_t));
        offset += 8*sizeof(uint8_t);

        memcpy(buf + offset, &WBSystemBandwidth, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &SystemPower, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &Spare2, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &SerialNumber, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(buf + offset, &BeamAngle, sizeof(uint8_t));
        offset += sizeof(uint8_t);    
        
        return (offset > bytes ? -1 : offset);
}

int RDIFixedLeader::deserialise(const char* buf, const int bytes)
{
        int offset = 0;

        // ID is const
        offset += sizeof(uint16_t);

        memcpy(&CpuFirmwareVer, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&CpuFirmwareRev, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&SystemConfig, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&RealOrSim, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&LagLengthCm, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&NumberOfBeams, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&NumberOfCells, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&PingsPerEnsemble, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&DepthCellLengthCm, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&BlankAfterTransmitCm, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&ProfilingMode, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&LowcorrThresh, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&CodeRepeats, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&PercentGood, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&ErrorVelMax, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&TPPMins, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&TPPSecs, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&TPPHunds, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&CoordTxfrm, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&HeadingAlignment, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&HeadingBias, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&SensorSource, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&SensorAvail, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&Bin1DistanceCm, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&TxPulseLengthCm, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&StartEndDepthCell, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&FalseTargetThresh, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&Spare1, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&TxLagDistanceCm, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&Spares[0], buf + offset, 8*sizeof(uint8_t));
        offset += 8*sizeof(uint8_t);

        memcpy(&WBSystemBandwidth, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&SystemPower, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&Spare2, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&SerialNumber, buf + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(&BeamAngle, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);        
        
        return (offset > bytes ? -1 : offset);
}

RDIVariableLeader::RDIVariableLeader() :
        EnsembleNumber(0),
        RTCTSYear(0),
        RTCTSMonth(0),
        RTCTSDay(0),
        RTCTSHour(0),                       
        RTCTSMin(0),                         
        RTCTSSec(0),                           
        RTCTSHSec(0),                        
        EnsembleMSB(0),                       
        BITResult(0),                     
        SpeedOfSoundMs(0),                 
        DepthOfTxdrDm(0),                    
        HeadingCD(0),                        
        PitchCD(0),                 
        RollCD(0),                  
        SalinityPPT(0),           
        TempCD(0),                
        MptMins(0),              
        MptSecs(0),              
        MptHsecs(0),             
        HeadingSdev(0),         
        PitchSdev(0),           
        RollSdev(0),            
        AdcChan0(0),            
        AdcChan1(0),            
        AdcChan2(0),            
        AdcChan3(0),            
        AdcChan4(0),            
        AdcChan5(0),            
        AdcChan6(0),            
        AdcChan7(0),            
        StatusWord(0),          
        PressureDP(0),   
        PressureVarDP(0),
        RTCCentury(0),   
        RTCYear(0),      
        RTCMonth(0),     
        RTCDay(0),       
        RTCHour(0),      
        RTCMin(0),       
        RTCSec(0),       
        RTCHSec(0)  
{
        memset(&Spare[0], 0x0, sizeof(Spare));
}

RDIVariableLeader::RDIVariableLeader(const RDIVariableLeader& msg) :
        EnsembleNumber(msg.EnsembleNumber),
        RTCTSYear(msg.RTCTSYear),
        RTCTSMonth(msg.RTCTSMonth),
        RTCTSDay(msg.RTCTSDay),
        RTCTSHour(msg.RTCTSHour),                       
        RTCTSMin(msg.RTCTSMin),                         
        RTCTSSec(msg.RTCTSSec),                           
        RTCTSHSec(msg.RTCTSHSec),                        
        EnsembleMSB(msg.EnsembleMSB),                       
        BITResult(msg.BITResult),                     
        SpeedOfSoundMs(msg.SpeedOfSoundMs),                 
        DepthOfTxdrDm(msg.DepthOfTxdrDm),                    
        HeadingCD(msg.HeadingCD),                        
        PitchCD(msg.PitchCD),                 
        RollCD(msg.RollCD),                  
        SalinityPPT(msg.SalinityPPT),           
        TempCD(msg.TempCD),                
        MptMins(msg.MptMins),              
        MptSecs(msg.MptSecs),              
        MptHsecs(msg.MptHsecs),             
        HeadingSdev(msg.HeadingSdev),         
        PitchSdev(msg.PitchSdev),           
        RollSdev(msg.RollSdev),            
        AdcChan0(msg.AdcChan0),            
        AdcChan1(msg.AdcChan1),            
        AdcChan2(msg.AdcChan2),            
        AdcChan3(msg.AdcChan3),            
        AdcChan4(msg.AdcChan4),            
        AdcChan5(msg.AdcChan5),            
        AdcChan6(msg.AdcChan6),            
        AdcChan7(msg.AdcChan7),            
        StatusWord(msg.StatusWord),          
        PressureDP(msg.PressureDP),   
        PressureVarDP(msg.PressureVarDP),
        RTCCentury(msg.RTCCentury),   
        RTCYear(msg.RTCYear),      
        RTCMonth(msg.RTCMonth),     
        RTCDay(msg.RTCDay),       
        RTCHour(msg.RTCHour),      
        RTCMin(msg.RTCMin),       
        RTCSec(msg.RTCSec),       
        RTCHSec(msg.RTCHSec)
{
        memcpy(&Spare[0], &msg.Spare[0], sizeof(Spare));    
}

RDIVariableLeader::~RDIVariableLeader()
{

}

size_t RDIVariableLeader::calc_length() const
{
    // PD0 fields binary by default
    return SIZE_BYTES;
}

int RDIVariableLeader::serialise(char* buf, const int bytes) const
{
        int offset = 0;

        memcpy(buf + offset, &VariableLeaderID, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &EnsembleNumber, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &RTCTSYear, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &RTCTSMonth, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &RTCTSDay, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &RTCTSHour, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &RTCTSMin, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &RTCTSSec, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &RTCTSHSec, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &EnsembleMSB, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &BITResult, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &SpeedOfSoundMs, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &DepthOfTxdrDm, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &HeadingCD, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &PitchCD, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(buf + offset, &RollCD, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(buf + offset, &SalinityPPT, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &TempCD, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &MptMins, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &MptSecs, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &MptHsecs, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &HeadingSdev, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &PitchSdev, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &RollSdev, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &AdcChan0, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &AdcChan1, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &AdcChan2, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &AdcChan3, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &AdcChan4, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &AdcChan5, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &AdcChan6, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &AdcChan7, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &StatusWord, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(buf + offset, &PressureDP, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(buf + offset, &PressureVarDP, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(buf + offset, &Spare[0], 3*sizeof(uint8_t));
        offset += 3*sizeof(uint8_t);

        memcpy(buf + offset, &RTCCentury, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &RTCYear, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &RTCMonth, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &RTCDay, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &RTCHour, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &RTCMin, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &RTCSec, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &RTCHSec, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        return (offset > bytes ? -1 : offset);
}

int RDIVariableLeader::deserialise(const char* buf, const int bytes)
{
        int offset = 0;

        // ID is const
        offset += sizeof(uint16_t);

        memcpy(&EnsembleNumber, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&RTCTSYear, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&RTCTSMonth, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&RTCTSDay, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&RTCTSHour, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&RTCTSMin, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&RTCTSSec, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&RTCTSHSec, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&EnsembleMSB, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&BITResult, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&SpeedOfSoundMs, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&DepthOfTxdrDm, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&HeadingCD, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&PitchCD, buf + offset, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(&RollCD, buf + offset, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(&SalinityPPT, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&TempCD, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&MptMins, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&MptSecs, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&MptHsecs, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&HeadingSdev, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&PitchSdev, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&RollSdev, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&AdcChan0, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&AdcChan1, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&AdcChan2, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&AdcChan3, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&AdcChan4, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&AdcChan5, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&AdcChan6, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&AdcChan7, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&StatusWord, buf + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(&PressureDP, buf + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(&PressureVarDP, buf + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(&Spare[0], buf + offset, 3*sizeof(uint8_t));
        offset += 3*sizeof(uint8_t);

        memcpy(&RTCCentury, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&RTCYear, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&RTCMonth, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&RTCDay, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&RTCHour, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&RTCMin, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&RTCSec, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&RTCHSec, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);
        
        return (offset > bytes ? -1 : offset);
}

RDIVelocity::RDIVelocity(uint8_t number_of_cells) :
        _number_of_cells(number_of_cells)
{
        VelocitiesMms.resize(_number_of_beams * _number_of_cells);
        BaseFrameVelocitiesMms.resize(_number_of_beams * _number_of_cells);
}


RDIVelocity::RDIVelocity(const RDIVelocity& msg) :
        _number_of_cells(msg._number_of_cells),
        VelocitiesMms(msg.VelocitiesMms),
        BaseFrameVelocitiesMms(msg.BaseFrameVelocitiesMms)
{
    
}

RDIVelocity::~RDIVelocity()
{

}

size_t RDIVelocity::calc_length() const
{
        int size = 0;

        size += sizeof(Header);
        size += VelocitiesMms.size()*sizeof(int16_t);

        return size;    
}

int RDIVelocity::serialise(char* buf, const int bytes) const
{
        int offset = 0;

        memcpy(buf + offset, &Header, sizeof(Header));
        offset += sizeof(Header);

        memcpy(buf + offset, VelocitiesMms.data(), VelocitiesMms.size()*sizeof(int16_t));
        offset += VelocitiesMms.size()*sizeof(int16_t);            

        return offset;  
}

int RDIVelocity::deserialise(const char* buf, const int bytes)
{
        int offset = 0;

        // ID is const
        offset += sizeof(Header);

        memcpy(&VelocitiesMms[0], buf + offset, _number_of_beams * _number_of_cells * sizeof(int16_t));
        offset += VelocitiesMms.size()*sizeof(int16_t);            
        
        // This should really be dependent on decoded frame of reference
        BaseFrameVelocitiesMms = VelocitiesMms;
        
        return (offset > bytes ? -1 : offset);
}

void RDIVelocity::set(const uint8_t& number_of_cells)
{
        _number_of_cells = number_of_cells;

        VelocitiesMms.resize(_number_of_beams*_number_of_cells);
        BaseFrameVelocitiesMms.resize(_number_of_beams*_number_of_cells);
}

void RDIVelocity::subtractVelocities(std::vector<float> vesselMotionVelocities, bool reverse)
{
        // index array
        std::vector<uint16_t> beam_index(_number_of_beams, 0);
        
        for (int i = 0; i < _number_of_cells; ++i)
        {
                int cell_offset = i * _number_of_beams;
            
                // Subtract vessel motion velocities
                for (int j = 0; j < _number_of_beams; ++j)
                {
                        beam_index[j] = cell_offset + j;
                        if (!reverse) {
                                BaseFrameVelocitiesMms[beam_index[j]] -= vesselMotionVelocities[j];
                        } else {
                                BaseFrameVelocitiesMms[beam_index[j]] += vesselMotionVelocities[j];
                        }
                        
                }
        }
}

void RDIVelocity::transform(const EFrame& eFrame, const EFrame& BaseFrame, const RDIFixedLeader* cFixedLeader, const RDIVariableLeader* cVariableLeader)
{

        // factors to convert from beam frame to instrument frame - assumes four beams
        float a = 1 / (2 * sin(cFixedLeader->BeamAngle * f32DegToRadFactor));
        float b = 1 / (4 * cos(cFixedLeader->BeamAngle * f32DegToRadFactor));
        float c = 1;
        float d = a / sqrt(2);
        
        // factors to convert from instrument frame to vessel frame
        float sin_yawoffset = sin(cFixedLeader->HeadingAlignment * f32DegToRadFactor);
        float cos_yawoffset = cos(cFixedLeader->HeadingAlignment * f32DegToRadFactor);
        
        float sin_heading   = sin(+cVariableLeader->HeadingCD * 0.01 * f32DegToRadFactor);
        float cos_heading   = cos(+cVariableLeader->HeadingCD * 0.01 * f32DegToRadFactor);

        float sin_pitch     = sin(-cVariableLeader->PitchCD * 0.01 * f32DegToRadFactor);
        float cos_pitch     = cos(-cVariableLeader->PitchCD * 0.01 * f32DegToRadFactor);

        float sin_roll      = sin((-cVariableLeader->RollCD * 0.01 + 180) * f32DegToRadFactor);
        float cos_roll      = cos((-cVariableLeader->RollCD * 0.01 + 180) * f32DegToRadFactor);
        
        // index array
        std::vector<uint16_t> beam_index(cFixedLeader->NumberOfBeams, 0);

        // we only support transform to frames "above" the base frame in enum order.
        if (eFrame >= BaseFrame) {
        for (int i = 0; i < cFixedLeader->NumberOfCells; ++i)
        {
                int cell_offset = i * cFixedLeader->NumberOfBeams;
            
                // first, reset all beams to be base-frame
                for (int j = 0; j < cFixedLeader->NumberOfBeams; ++j)
                {
                        beam_index[j] = cell_offset + j;

                        VelocitiesMms[beam_index[j]] = BaseFrameVelocitiesMms[beam_index[j]];
                }
                    
                if (BaseFrame == EFrame::BEAM)
                {
                    if (eFrame == EFrame::BEAM)
                    {
                            continue;
                    }

                    // transform to instrument frame

                    if (cFixedLeader->NumberOfBeams > 3 )
                    {

                            // use beams 1-4 to calculate instrument x, y, z
                            int16_t vx = c* a * (VelocitiesMms[beam_index[0]] - VelocitiesMms[beam_index[1]]); // 1 - 2

                            int16_t vy = c* a * (VelocitiesMms[beam_index[3]] - VelocitiesMms[beam_index[2]]); // 4 - 3

                            int16_t vz = b *    (VelocitiesMms[beam_index[0]] + VelocitiesMms[beam_index[1]] + 
                                                 VelocitiesMms[beam_index[2]] + VelocitiesMms[beam_index[3]]); // 1 + 2 + 3 + 4

                            int16_t ve = d *    (VelocitiesMms[beam_index[0]] + VelocitiesMms[beam_index[1]] - 
                                                 VelocitiesMms[beam_index[2]] - VelocitiesMms[beam_index[3]]); // 1 + 2 - 3 - 4

                            VelocitiesMms[beam_index[0]] = vx;
                            VelocitiesMms[beam_index[1]] = vy;
                            VelocitiesMms[beam_index[2]] = vz;
                            VelocitiesMms[beam_index[3]] = ve;
                    }
                }
                if (BaseFrame < EFrame::VESSEL)
                {
                    if (eFrame == EFrame::INSTRUMENT)
                    {
                            continue;
                    }
                }

                int16_t v[3] = { VelocitiesMms[beam_index[0]], VelocitiesMms[beam_index[1]], VelocitiesMms[beam_index[2]] };

                if (BaseFrame < EFrame::VESSEL)
                {
                    // transform to vessel - yaw offset only
                    //order of rotations depend on sign of yaw offset
                    ::rotz(cos_yawoffset, sin_yawoffset, v);
                    ::roty(-1, 0, v); //rotation of 180 degrees about y-axis

                    for (int j = 0; j < 3; ++j)
                    {
                            VelocitiesMms[beam_index[j]] = v[j];
                    }

                    // beam 4 index kept as error velocity
                }
                if (BaseFrame < EFrame::EARTH)
                {
                    if (eFrame == EFrame::VESSEL)
                    {
                            continue;
                    }
            
                    // might need a copy of x and y components to rotate 5th beam                
                    int16_t vVesselCopy[3] = { v[0], v[1], v[2]};                
                
                    // transform
                    ::roty(cos_roll,    sin_roll,    v);
                    ::rotx(cos_pitch,   sin_pitch,   v);
                    ::rotz(cos_heading, sin_heading, v);

                    for (int j = 0; j < 3; ++j)
                    {
                            VelocitiesMms[beam_index[j]] = v[j];
                    }
                }
        }
        }
}

RDIXC::RDIXC(uint8_t number_of_cells) :
        _number_of_cells(number_of_cells)
{
        BeamXCs.resize(_number_of_beams * _number_of_cells);
}

RDIXC::RDIXC(const RDIXC& msg) :
        _number_of_cells(msg._number_of_cells),
        BeamXCs(msg.BeamXCs)
{
    
}

RDIXC::~RDIXC()
{

}

size_t RDIXC::calc_length() const
{
        int size = 0;

        size += sizeof(Header);
        size += BeamXCs.size()*sizeof(uint8_t);

        return size;        
}

int RDIXC::serialise(char* buf, const int bytes) const
{
        int offset = 0;

        memcpy(buf + offset, &Header, sizeof(Header));
        offset += sizeof(Header);

        memcpy(buf + offset, BeamXCs.data(), BeamXCs.size()*sizeof(uint8_t));
        offset += BeamXCs.size()*sizeof(uint8_t);            

        return (offset > bytes ? -1 : offset);
}

int RDIXC::deserialise(const char* buf, const int bytes)
{
        int offset = 0;

        // ID is const
        offset += sizeof(Header);

        memcpy(&BeamXCs[0], buf + offset, _number_of_beams * _number_of_cells * sizeof(uint8_t));
        offset += BeamXCs.size()*sizeof(uint8_t);            
        
        return (offset > bytes ? -1 : offset);    
}

void RDIXC::set(const uint8_t& number_of_cells)
{
        _number_of_cells = number_of_cells;

        BeamXCs.resize(_number_of_beams*_number_of_cells);
}

RDIIntensity::RDIIntensity(uint8_t number_of_cells) :
        _number_of_cells(number_of_cells)
{
        BeamIntensities.resize(_number_of_beams * _number_of_cells);
}

RDIIntensity::RDIIntensity(const RDIIntensity& msg) :
        _number_of_cells(msg._number_of_cells),
        BeamIntensities(msg.BeamIntensities)
{
    
}

RDIIntensity::~RDIIntensity()
{

}

size_t RDIIntensity::calc_length() const
{
        int size = 0;

        size += sizeof(Header);
        size += BeamIntensities.size()*sizeof(uint8_t);

        return size;            
}

int RDIIntensity::serialise(char* buf, const int bytes) const
{
        int offset = 0;

        memcpy(buf + offset, &Header, sizeof(Header));
        offset += sizeof(Header);

        memcpy(buf + offset, BeamIntensities.data(), BeamIntensities.size()*sizeof(uint8_t));
        offset += BeamIntensities.size()*sizeof(uint8_t);            

        return (offset > bytes ? -1 : offset);
}

int RDIIntensity::deserialise(const char* buf, const int bytes)
{
        int offset = 0;

        // ID is const
        offset += sizeof(Header);

        memcpy(&BeamIntensities[0], buf + offset, _number_of_beams * _number_of_cells * sizeof(uint8_t));
        offset += BeamIntensities.size()*sizeof(uint8_t);            
        
        return (offset > bytes ? -1 : offset);
}

void RDIIntensity::set(const uint8_t& number_of_cells)
{
        _number_of_cells = number_of_cells;

        BeamIntensities.resize(_number_of_beams * _number_of_cells);
}

RDIPrctGood::RDIPrctGood(uint8_t number_of_cells) :
        _number_of_cells(number_of_cells) 
{
        BeamPrctGood.resize(_number_of_beams * _number_of_cells);
}

RDIPrctGood::RDIPrctGood(const RDIPrctGood& msg) :
        _number_of_cells(msg._number_of_cells),
        BeamPrctGood(msg.BeamPrctGood)
{
    
}

RDIPrctGood::~RDIPrctGood()
{

}

size_t RDIPrctGood::calc_length() const
{
        int size = 0;

        size += sizeof(Header);
        size += BeamPrctGood.size()*sizeof(uint8_t);

        return size;            
}

int RDIPrctGood::serialise(char* buf, const int bytes) const
{
        int offset = 0;

        memcpy(buf + offset, &Header, sizeof(Header));
        offset += sizeof(Header);

        memcpy(buf + offset, BeamPrctGood.data(), BeamPrctGood.size()*sizeof(uint8_t));
        offset += BeamPrctGood.size()*sizeof(uint8_t);            

        return (offset > bytes ? -1 : offset);
}

int RDIPrctGood::deserialise(const char* buf, const int bytes)
{
        int offset = 0;

        // ID is const
        offset += sizeof(Header);

        memcpy(&BeamPrctGood[0], buf + offset, _number_of_beams * _number_of_cells * sizeof(uint8_t));
        offset += BeamPrctGood.size()*sizeof(uint8_t);            
        
        return (offset > bytes ? -1 : offset);
}

void RDIPrctGood::set(const uint8_t& number_of_cells)
{
        _number_of_cells = number_of_cells;

        BeamPrctGood.resize(_number_of_beams*_number_of_cells);
}

RDIVerticalBeamLeader::RDIVerticalBeamLeader() :
        DepthCells(0),
        VerticalPings(0),
        DepthCellSizeCm(0),
        FirstCellRangeCm(0),
        VerticalMode(0),
        VerticalTransmitCm(0),
        VerticalLagLengthCm(0),
        TransmitCodeEls(0),
        VertRSSIThresh(0),
        VertShallowBin(0),
        VertStartBin(0),
        VertShallowRSSIBin(0),
        MaxCoreThreshold(0),
        MinCoreThreshold(0),
        PingOffsetTimeMs(0),
        Spare1(0),
        DepthScreen(0),
        PercentGoodThresh(0),
        VerticalDOProofing(0)
{

}

RDIVerticalBeamLeader::RDIVerticalBeamLeader(const RDIVerticalBeamLeader& msg) :
        DepthCells(msg.DepthCells),
        VerticalPings(msg.VerticalPings),
        DepthCellSizeCm(msg.DepthCellSizeCm),
        FirstCellRangeCm(msg.FirstCellRangeCm),
        VerticalMode(msg.VerticalMode),
        VerticalTransmitCm(msg.VerticalTransmitCm),
        VerticalLagLengthCm(msg.VerticalLagLengthCm),
        TransmitCodeEls(msg.TransmitCodeEls),
        VertRSSIThresh(msg.VertRSSIThresh),
        VertShallowBin(msg.VertShallowBin),
        VertStartBin(msg.VertStartBin),
        VertShallowRSSIBin(msg.VertShallowRSSIBin),
        MaxCoreThreshold(msg.MaxCoreThreshold),
        MinCoreThreshold(msg.MinCoreThreshold),
        PingOffsetTimeMs(msg.PingOffsetTimeMs),
        Spare1(msg.Spare1),
        DepthScreen(msg.DepthScreen),
        PercentGoodThresh(msg.PercentGoodThresh),
        VerticalDOProofing(msg.VerticalDOProofing)
{

}

RDIVerticalBeamLeader::~RDIVerticalBeamLeader()
{

}

size_t RDIVerticalBeamLeader::calc_length() const
{
    // PD0 fields binary by default
    return SIZE_BYTES;
}

int RDIVerticalBeamLeader::serialise(char* buf, const int bytes) const
{
    int offset = 0;

    memcpy(buf + offset, &VerticalBeamLeaderID, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &DepthCells, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &VerticalPings, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &DepthCellSizeCm, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &FirstCellRangeCm, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &VerticalMode, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &VerticalTransmitCm, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &VerticalLagLengthCm, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &TransmitCodeEls, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &VertRSSIThresh, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &VertShallowBin, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &VertStartBin, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &VertShallowRSSIBin, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &MaxCoreThreshold, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &MinCoreThreshold, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &PingOffsetTimeMs, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(buf + offset, &Spare1, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &DepthScreen, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &PercentGoodThresh, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &VerticalDOProofing, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    return (offset > bytes ? -1 : offset);
}

int RDIVerticalBeamLeader::deserialise(const char* buf, const int bytes)
{
    int offset = 0;

    // ID is const
    offset += sizeof(uint16_t);

    memcpy(&DepthCells, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&VerticalPings, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&DepthCellSizeCm, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&FirstCellRangeCm, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&VerticalMode, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&VerticalTransmitCm, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&VerticalLagLengthCm, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&TransmitCodeEls, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&VertRSSIThresh, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&VertShallowBin, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&VertStartBin, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&VertShallowRSSIBin, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&MaxCoreThreshold, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&MinCoreThreshold, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&PingOffsetTimeMs, buf + offset, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(&Spare1, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&DepthScreen, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&PercentGoodThresh, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&VerticalDOProofing, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    return (offset > bytes ? -1 : offset);
}

RDIVerticalBeamVelocity::RDIVerticalBeamVelocity(uint8_t number_of_cells) :
    _number_of_cells(number_of_cells)
{
    VelocitiesMms.resize(_number_of_cells);
    BaseFrameVelocitiesMms.resize(_number_of_cells);
}

RDIVerticalBeamVelocity::RDIVerticalBeamVelocity(const RDIVerticalBeamVelocity &msg) :
    _number_of_cells(msg._number_of_cells),
    VelocitiesMms(msg.VelocitiesMms),
    BaseFrameVelocitiesMms(msg.BaseFrameVelocitiesMms)
{

}

RDIVerticalBeamVelocity::~RDIVerticalBeamVelocity()
{

}

size_t RDIVerticalBeamVelocity::calc_length() const
{
    int size = 0;

    size += sizeof(Header);
    size += VelocitiesMms.size()*sizeof(int16_t);

    return size;
}

int RDIVerticalBeamVelocity::serialise(char *buf, const int bytes) const
{
    int offset = 0;

    memcpy(buf + offset, &Header, sizeof(Header));
    offset += sizeof(Header);

    memcpy(buf + offset, VelocitiesMms.data(), VelocitiesMms.size()*sizeof(int16_t));
    offset += VelocitiesMms.size()*sizeof(int16_t);

    return (offset > bytes ? -1 : offset);
}

int RDIVerticalBeamVelocity::deserialise(const char *buf, const int bytes)
{
    int offset = 0;

    // ID is const
    offset += sizeof(Header);

    memcpy(&VelocitiesMms[0], buf + offset, _number_of_cells * sizeof(int16_t));
    offset += VelocitiesMms.size()*sizeof(int16_t);

    BaseFrameVelocitiesMms = VelocitiesMms;

    return (offset > bytes ? -1 : offset);
}

void RDIVerticalBeamVelocity::set(const uint8_t &number_of_cells)
{
    _number_of_cells = number_of_cells;

    VelocitiesMms.resize(_number_of_cells);
    BaseFrameVelocitiesMms.resize(_number_of_cells);
}

std::vector<int16_t>& RDIVerticalBeamVelocity::get_BeamVelocitiesMms()
{
    return BaseFrameVelocitiesMms;
}

RDIVerticalBeamXC::RDIVerticalBeamXC(uint8_t number_of_cells) :
    _number_of_cells(number_of_cells)
{
    BeamXCs.resize(_number_of_cells);
}

RDIVerticalBeamXC::RDIVerticalBeamXC(const RDIVerticalBeamXC& msg) :
        _number_of_cells(msg._number_of_cells),
        BeamXCs(msg.BeamXCs)
{

}

RDIVerticalBeamXC::~RDIVerticalBeamXC()
{

}

size_t RDIVerticalBeamXC::calc_length() const
{
    int size = 0;

    size += sizeof(Header);
    size += BeamXCs.size()*sizeof(uint8_t);

    return size;
}

int RDIVerticalBeamXC::serialise(char* buf, const int bytes) const
{
    int offset = 0;

    memcpy(buf + offset, &Header, sizeof(Header));
    offset += sizeof(Header);

    memcpy(buf + offset, BeamXCs.data(), BeamXCs.size()*sizeof(uint8_t));
    offset += BeamXCs.size()*sizeof(uint8_t);

    return (offset > bytes ? -1 : offset);
}

int RDIVerticalBeamXC::deserialise(const char* buf, const int bytes)
{
    int offset = 0;

    // ID is const
    offset += sizeof(Header);

    memcpy(&BeamXCs[0], buf + offset, _number_of_cells * sizeof(uint8_t));
    offset += BeamXCs.size()*sizeof(uint8_t);

    return (offset > bytes ? -1 : offset);
}

void RDIVerticalBeamXC::set(const uint8_t& number_of_cells)
{
    _number_of_cells = number_of_cells;

    BeamXCs.resize(number_of_cells);
}

std::vector<uint8_t>& RDIVerticalBeamXC::get_BeamXCs()
{
    return BeamXCs;
}

RDIVerticalBeamIntensity::RDIVerticalBeamIntensity(uint8_t number_of_cells) :
    _number_of_cells(number_of_cells)
{
    BeamIntensities.resize(_number_of_cells);
}

RDIVerticalBeamIntensity::RDIVerticalBeamIntensity(const RDIVerticalBeamIntensity &msg) :
    _number_of_cells(msg._number_of_cells),
    BeamIntensities(msg.BeamIntensities)
{

}

RDIVerticalBeamIntensity::~RDIVerticalBeamIntensity()
{

}

size_t RDIVerticalBeamIntensity::calc_length() const
{
    int size = 0;

    size += sizeof(Header);
    size += BeamIntensities.size()*sizeof(uint8_t);

    return size;
}

int RDIVerticalBeamIntensity::serialise(char *buf, const int bytes) const
{
    int offset = 0;

    memcpy(buf + offset, &Header, sizeof(Header));
    offset += sizeof(Header);

    memcpy(buf + offset, BeamIntensities.data(), BeamIntensities.size()*sizeof(uint8_t));
    offset += BeamIntensities.size()*sizeof(uint8_t);

    return (offset > bytes ? -1 : offset);
}

int RDIVerticalBeamIntensity::deserialise(const char* buf, const int bytes)
{
    int offset = 0;

    // ID is const
    offset += sizeof(Header);

    memcpy(&BeamIntensities[0], buf + offset, _number_of_cells * sizeof(uint8_t));
    offset += BeamIntensities.size()*sizeof(uint8_t);

    return (offset > bytes ? -1 : offset);
}

void RDIVerticalBeamIntensity::set(const uint8_t& number_of_cells)
{
    _number_of_cells = number_of_cells;

    BeamIntensities.resize(_number_of_cells);
}

std::vector<uint8_t>& RDIVerticalBeamIntensity::get_BeamIntensities()
{
    return BeamIntensities;
}

RDIVerticalBeamPrctGood::RDIVerticalBeamPrctGood(uint8_t number_of_cells) :
    _number_of_cells(number_of_cells)
{
    BeamPrctGood.resize(_number_of_cells);
}

RDIVerticalBeamPrctGood::RDIVerticalBeamPrctGood(const RDIVerticalBeamPrctGood &msg) :
        _number_of_cells(msg._number_of_cells),
        BeamPrctGood(msg.BeamPrctGood)
{

}

RDIVerticalBeamPrctGood::~RDIVerticalBeamPrctGood()
{

}

size_t RDIVerticalBeamPrctGood::calc_length() const
{
    int size = 0;

    size += sizeof(Header);
    size += BeamPrctGood.size()*sizeof(uint8_t);

    return size;
}

int RDIVerticalBeamPrctGood::serialise(char *buf, const int bytes) const
{
    int offset = 0;

    memcpy(buf + offset, &Header, sizeof(Header));
    offset += sizeof(Header);

    memcpy(buf + offset, BeamPrctGood.data(), BeamPrctGood.size()*sizeof(uint8_t));
    offset += BeamPrctGood.size()*sizeof(uint8_t);

    return (offset > bytes ? -1 : offset);
}

int RDIVerticalBeamPrctGood::deserialise(const char* buf, const int bytes)
{
    int offset = 0;

    // ID is const
    offset += sizeof(Header);

    memcpy(&BeamPrctGood[0], buf + offset, _number_of_cells * sizeof(uint8_t));
    offset += BeamPrctGood.size()*sizeof(uint8_t);

    return (offset > bytes ? -1 : offset);
}

void RDIVerticalBeamPrctGood::set(const uint8_t& number_of_cells)
{
    _number_of_cells = number_of_cells;

    BeamPrctGood.resize(_number_of_cells);
}

std::vector<uint8_t>& RDIVerticalBeamPrctGood::get_BeamPrctGood()
{
    return BeamPrctGood;
}

RDIBottomTrack::RDIBottomTrack() :
        BTPingsPerEnsemble(0),
        BTDelayBeforeReaquire(0),
        BTCorrMagMin(0),
        BTEvalAmpMin(0),
        BTPercentGood(0),
        BTMode(0),
        BTErrVelMax(0),
        BTReserved(0),
        BTBeam0RangeCm(0),
        BTBeam1RangeCm(0),
        BTBeam2RangeCm(0),
        BTBeam3RangeCm(0),
        BTBeam0VelMms(0),
        BTBeam1VelMms(0),
        BTBeam2VelMms(0),
        BTBeam3VelMms(0),
        BTBeam0Corr(0),
        BTBeam1Corr(0),
        BTBeam2Corr(0),
        BTBeam3Corr(0),
        BTBeam0Amp(0),
        BTBeam1Amp(0),
        BTBeam2Amp(0),
        BTBeam3Amp(0),
        BTBeam0PercentGood(0),
        BTBeam1PercentGood(0),
        BTBeam2PercentGood(0),
        BTBeam3PercentGood(0),
        RefLayerMinDm(0),
        RefLayerNearDm(0),
        RefLayerFarDm(0),
        BTRefLayerVelBeam0Mms(0),
        BTRefLayerVelBeam1Mms(0),
        BTRefLayerVelBeam2Mms(0),
        BTRefLayerVelBeam3Mms(0),
        BTBeam0RefLayerCorr(0),
        BTBeam1RefLayerCorr(0),
        BTBeam2RefLayerCorr(0),
        BTBeam3RefLayerCorr(0),
        BTBeam0RefLayerIntensity(0),
        BTBeam1RefLayerIntensity(0),
        BTBeam2RefLayerIntensity(0),
        BTBeam3RefLayerIntensity(0),
        BTRefPercentGoodBeam0(0),
        BTRefPercentGoodBeam1(0),
        BTRefPercentGoodBeam2(0),
        BTRefPercentGoodBeam3(0),
        BTMaxDepthDm(0),
        BTRSSIBeam0(0),
        BTRSSIBeam1(0),
        BTRSSIBeam2(0),
        BTRSSIBeam3(0),
        Gain(0),
        BTBeam0RangeCmMbs(0),
        BTBeam1RangeCmMbs(0),
        BTBeam2RangeCmMbs(0),
        BTBeam3RangeCmMbs(0)
{
    
}

RDIBottomTrack::RDIBottomTrack(const RDIBottomTrack& msg) :
        BTPingsPerEnsemble(msg.BTPingsPerEnsemble),
        BTDelayBeforeReaquire(msg.BTDelayBeforeReaquire),
        BTCorrMagMin(msg.BTCorrMagMin),
        BTEvalAmpMin(msg.BTEvalAmpMin),
        BTPercentGood(msg.BTPercentGood),
        BTMode(msg.BTMode),
        BTErrVelMax(msg.BTErrVelMax),
        BTReserved(msg.BTReserved),
        BTBeam0RangeCm(msg.BTBeam0RangeCm),
        BTBeam1RangeCm(msg.BTBeam1RangeCm),
        BTBeam2RangeCm(msg.BTBeam2RangeCm),
        BTBeam3RangeCm(msg.BTBeam3RangeCm),
        BTBeam0VelMms(msg.BTBeam0VelMms),
        BTBeam1VelMms(msg.BTBeam1VelMms),
        BTBeam2VelMms(msg.BTBeam2VelMms),
        BTBeam3VelMms(msg.BTBeam3VelMms),
        BTBeam0Corr(msg.BTBeam0Corr),
        BTBeam1Corr(msg.BTBeam1Corr),
        BTBeam2Corr(msg.BTBeam2Corr),
        BTBeam3Corr(msg.BTBeam3Corr),
        BTBeam0Amp(msg.BTBeam0Amp),
        BTBeam1Amp(msg.BTBeam1Amp),
        BTBeam2Amp(msg.BTBeam2Amp),
        BTBeam3Amp(msg.BTBeam3Amp),
        BTBeam0PercentGood(msg.BTBeam0PercentGood),
        BTBeam1PercentGood(msg.BTBeam1PercentGood),
        BTBeam2PercentGood(msg.BTBeam2PercentGood),
        BTBeam3PercentGood(msg.BTBeam3PercentGood),
        RefLayerMinDm(msg.RefLayerMinDm),
        RefLayerNearDm(msg.RefLayerNearDm),
        RefLayerFarDm(msg.RefLayerFarDm),
        BTRefLayerVelBeam0Mms(msg.BTRefLayerVelBeam0Mms),
        BTRefLayerVelBeam1Mms(msg.BTRefLayerVelBeam1Mms),
        BTRefLayerVelBeam2Mms(msg.BTRefLayerVelBeam2Mms),
        BTRefLayerVelBeam3Mms(msg.BTRefLayerVelBeam3Mms),
        BTBeam0RefLayerCorr(msg.BTBeam0RefLayerCorr),
        BTBeam1RefLayerCorr(msg.BTBeam1RefLayerCorr),
        BTBeam2RefLayerCorr(msg.BTBeam2RefLayerCorr),
        BTBeam3RefLayerCorr(msg.BTBeam3RefLayerCorr),
        BTBeam0RefLayerIntensity(msg.BTBeam0RefLayerIntensity),
        BTBeam1RefLayerIntensity(msg.BTBeam1RefLayerIntensity),
        BTBeam2RefLayerIntensity(msg.BTBeam2RefLayerIntensity),
        BTBeam3RefLayerIntensity(msg.BTBeam3RefLayerIntensity),
        BTRefPercentGoodBeam0(msg.BTRefPercentGoodBeam0),
        BTRefPercentGoodBeam1(msg.BTRefPercentGoodBeam1),
        BTRefPercentGoodBeam2(msg.BTRefPercentGoodBeam2),
        BTRefPercentGoodBeam3(msg.BTRefPercentGoodBeam3),
        BTMaxDepthDm(msg.BTMaxDepthDm),
        BTRSSIBeam0(msg.BTRSSIBeam0),
        BTRSSIBeam1(msg.BTRSSIBeam1),
        BTRSSIBeam2(msg.BTRSSIBeam2),
        BTRSSIBeam3(msg.BTRSSIBeam3),
        Gain(msg.Gain),
        BTBeam0RangeCmMbs(msg.BTBeam0RangeCmMbs),
        BTBeam1RangeCmMbs(msg.BTBeam1RangeCmMbs),
        BTBeam2RangeCmMbs(msg.BTBeam2RangeCmMbs),
        BTBeam3RangeCmMbs(msg.BTBeam3RangeCmMbs)
{
    
}

RDIBottomTrack::~RDIBottomTrack()
{

}

size_t RDIBottomTrack::calc_length() const
{
        return SIZE_BYTES;       
}

int RDIBottomTrack::serialise(char* buf, const int bytes) const
{
    int offset = 0;
    
    memcpy(buf + offset, &BottomTrackID, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &BTPingsPerEnsemble, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &BTDelayBeforeReaquire, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &BTCorrMagMin, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTEvalAmpMin, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTPercentGood, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTMode, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTErrVelMax, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &BTReserved, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(buf + offset, &BTBeam0RangeCm, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &BTBeam1RangeCm, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &BTBeam2RangeCm, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &BTBeam3RangeCm, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &BTBeam0VelMms, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(buf + offset, &BTBeam1VelMms, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(buf + offset, &BTBeam2VelMms, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(buf + offset, &BTBeam3VelMms, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(buf + offset, &BTBeam0Corr, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam1Corr, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam2Corr, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam3Corr, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam0Amp, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam1Amp, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam2Amp, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam3Amp, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam0PercentGood, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam1PercentGood, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam2PercentGood, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam3PercentGood, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &RefLayerMinDm, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &RefLayerNearDm, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &RefLayerFarDm, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &BTRefLayerVelBeam0Mms, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(buf + offset, &BTRefLayerVelBeam1Mms, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(buf + offset, &BTRefLayerVelBeam2Mms, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(buf + offset, &BTRefLayerVelBeam3Mms, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(buf + offset, &BTBeam0RefLayerCorr, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam1RefLayerCorr, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam2RefLayerCorr, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam3RefLayerCorr, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam0RefLayerIntensity, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam1RefLayerIntensity, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam2RefLayerIntensity, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam3RefLayerIntensity, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTRefPercentGoodBeam0, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTRefPercentGoodBeam1, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTRefPercentGoodBeam2, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTRefPercentGoodBeam3, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTMaxDepthDm, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &BTRSSIBeam0, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTRSSIBeam1, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTRSSIBeam2, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTRSSIBeam3, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &Gain, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam0RangeCmMbs, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam1RangeCmMbs, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam2RangeCmMbs, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &BTBeam3RangeCmMbs, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(buf + offset, &Reserved, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    return offset;
}

int RDIBottomTrack::deserialise(const char* buf, const int bytes)
{
    int offset = 0;
    
    //memcpy(&BottomTrackID, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&BTPingsPerEnsemble, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&BTDelayBeforeReaquire, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&BTCorrMagMin, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTEvalAmpMin, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTPercentGood, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTMode, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTErrVelMax, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&BTReserved, buf + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(&BTBeam0RangeCm, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&BTBeam1RangeCm, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&BTBeam2RangeCm, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&BTBeam3RangeCm, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&BTBeam0VelMms, buf + offset, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(&BTBeam1VelMms, buf + offset, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(&BTBeam2VelMms, buf + offset, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(&BTBeam3VelMms, buf + offset, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(&BTBeam0Corr, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam1Corr, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam2Corr, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam3Corr, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam0Amp, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam1Amp, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam2Amp, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam3Amp, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam0PercentGood, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam1PercentGood, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam2PercentGood, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam3PercentGood, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&RefLayerMinDm, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&RefLayerNearDm, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&RefLayerFarDm, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&BTRefLayerVelBeam0Mms, buf + offset, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(&BTRefLayerVelBeam1Mms, buf + offset, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(&BTRefLayerVelBeam2Mms, buf + offset, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(&BTRefLayerVelBeam3Mms, buf + offset, sizeof(int16_t));
    offset += sizeof(int16_t);

    memcpy(&BTBeam0RefLayerCorr, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam1RefLayerCorr, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam2RefLayerCorr, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam3RefLayerCorr, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam0RefLayerIntensity, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam1RefLayerIntensity, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam2RefLayerIntensity, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam3RefLayerIntensity, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTRefPercentGoodBeam0, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTRefPercentGoodBeam1, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTRefPercentGoodBeam2, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTRefPercentGoodBeam3, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTMaxDepthDm, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&BTRSSIBeam0, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTRSSIBeam1, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTRSSIBeam2, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTRSSIBeam3, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&Gain, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam0RangeCmMbs, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam1RangeCmMbs, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam2RangeCmMbs, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&BTBeam3RangeCmMbs, buf + offset, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    memcpy(&Reserved, buf + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    return (offset > bytes ? -1 : offset);
}

RDINMEAGGABinary::RDINMEAGGABinary()
{
}


RDINMEAGGABinary::RDINMEAGGABinary(const RDINMEAGGABinary& msg):
        dLatitude(msg.dLatitude),
        tcNS(msg.tcNS),
        dLongitude(msg.dLongitude),
        tcEW(msg.tcEW),
        ucQuality(msg.ucQuality),
        ucNmbSat(msg.ucNmbSat),
        fHDOP(msg.fHDOP),
        fAltitude(msg.fAltitude),
        tcAltUnit(msg.tcAltUnit),
        fGeoid(msg.fGeoid),
        tcGeoidIUnit(msg.tcGeoidIUnit),
        fAgeDGPS(msg.fAgeDGPS),
        sRefStationId(msg.sRefStationId)
{
    memcpy(szHeader, msg.szHeader, sizeof(msg.szHeader));
    memcpy(szUTC, msg.szUTC, sizeof(msg.szUTC));
}

RDINMEAGGABinary::~RDINMEAGGABinary()
{

}

size_t RDINMEAGGABinary::calc_length() const
{
        int size = SIZE_BYTES;        

        return size;            
}

int RDINMEAGGABinary::serialise(char* buf, const int bytes) const
{
        int offset = 0;

        // serialise the binary header
        memcpy(buf + offset, &RDINMEABinaryID, sizeof(RDINMEABinaryID));
        offset += sizeof(RDINMEABinaryID);
        
        memcpy(buf + offset, &SID, sizeof(SID));
        offset += sizeof(SID);
        
        memcpy(buf + offset, &SIZE_BYTES, sizeof(SIZE_BYTES));
        offset += sizeof(SIZE_BYTES);
        
        // placeholder for delta time
        offset += RDINMEABINARYTIMESIZE;

        memcpy(buf + offset, &szHeader[0], sizeof(szHeader));
        offset += sizeof(szHeader);

        memcpy(buf + offset, &szUTC[0], sizeof(szUTC));
        offset += sizeof(szUTC);

        memcpy(buf + offset, &dLatitude, sizeof(double));
        offset += sizeof(double);

        memcpy(buf + offset, &tcNS, sizeof(tcNS));
        offset += sizeof(tcNS);

        memcpy(buf + offset, &dLongitude, sizeof(double));
        offset += sizeof(double);

        memcpy(buf + offset, &tcEW, sizeof(tcEW));
        offset += sizeof(tcEW);

        memcpy(buf + offset, &ucQuality, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &ucNmbSat, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &fHDOP, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &fAltitude, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &tcAltUnit, sizeof(char));
        offset += sizeof(char);

        memcpy(buf + offset, &fGeoid, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &tcGeoidIUnit, sizeof(char));
        offset += sizeof(char);

        memcpy(buf + offset, &fAgeDGPS, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &sRefStationId, sizeof(uint16_t));
        offset += sizeof(uint16_t);        
        
        return (offset > bytes ? -1 : offset);
}

int RDINMEAGGABinary::deserialise(const char* buf, const int bytes)
{
        int offset = 0;

        // serialise the binary header
        //memcpy(&RDINMEABinaryID, buf + offset, sizeof(RDINMEABinaryID));
        offset += sizeof(RDINMEABinaryID);
        
        //memcpy(&SID, buf + offset, sizeof(SID));
        offset += sizeof(SID);
        
        //memcpy(&SIZE_BYTES, buf + offset, sizeof(SIZE_BYTES));
        offset += sizeof(SIZE_BYTES);
        
        // placeholder for delta time
        offset += RDINMEABINARYTIMESIZE;

        memcpy(&szHeader[0], buf + offset, sizeof(szHeader));
        offset += sizeof(szHeader);

        memcpy(&szUTC[0], buf + offset, sizeof(szUTC));
        offset += sizeof(szUTC);

        memcpy(&dLatitude, buf + offset, sizeof(double));
        offset += sizeof(double);

        memcpy(&tcNS, buf + offset, sizeof(tcNS));
        offset += sizeof(tcNS);

        memcpy(&dLongitude, buf + offset, sizeof(double));
        offset += sizeof(double);

        memcpy(&tcEW, buf + offset, sizeof(tcEW));
        offset += sizeof(tcEW);

        memcpy(&ucQuality, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&ucNmbSat, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&fHDOP, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&fAltitude, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&tcAltUnit, buf + offset, sizeof(char));
        offset += sizeof(char);

        memcpy(&fGeoid, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&tcGeoidIUnit, buf + offset, sizeof(char));
        offset += sizeof(char);

        memcpy(&fAgeDGPS, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&sRefStationId, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);
        
        return (offset > bytes ? -1 : offset);
}

RDINMEAGGAString::RDINMEAGGAString()
{
}

RDINMEAGGAString::RDINMEAGGAString(const RDINMEAGGAString& msg)
{
    
}

RDINMEAGGAString::~RDINMEAGGAString()
{

}

size_t RDINMEAGGAString::calc_length() const
{
        int size = SIZE_BYTES;        

        return size;            
}

int RDINMEAGGAString::serialise(char* buf, const int bytes) const
{
        int offset = 0;
    
        memcpy(buf + offset, &IdCode, sizeof(uint16_t));
        offset += sizeof(uint16_t);
        
        return SIZE_BYTES;
}

int RDINMEAGGAString::deserialise(const char* buf, const int bytes)
{
        int offset = 14;

        // for now, do this:
        memcpy(&GGAString[0], buf + offset, sizeof(GGAString));
        offset += sizeof(GGAString);
        
        return (offset > bytes ? -1 : offset);
}

RDINMEAVTGBinary::RDINMEAVTGBinary()
{
}

RDINMEAVTGBinary::RDINMEAVTGBinary(const RDINMEAVTGBinary& msg) :
    fCOGTrue(msg.fCOGTrue),
    tcTrueIndicator(msg.tcTrueIndicator),
    fCOGMag(msg.fCOGMag),
    tcMagnIndicator(msg.tcMagnIndicator),
    fSpdOverGroundKts(msg.fSpdOverGroundKts),
    tcKtsIndicator(msg.tcKtsIndicator),
    fSpdOverGroundKmh(msg.fSpdOverGroundKmh),
    tcKmhIndicator(msg.tcKmhIndicator),
    tcModeIndicator(msg.tcModeIndicator)
{
    memcpy(szHeader, msg.szHeader, sizeof(msg.szHeader));
}

RDINMEAVTGBinary::~RDINMEAVTGBinary()
{

}

size_t RDINMEAVTGBinary::calc_length() const
{
        int size = SIZE_BYTES;        

        return size;            
}

int RDINMEAVTGBinary::serialise(char* buf, const int bytes) const
{
        int offset = 0;

        // serialise the binary header
        memcpy(buf + offset, &RDINMEABinaryID, sizeof(RDINMEABinaryID));
        offset += sizeof(RDINMEABinaryID);
        
        memcpy(buf + offset, &SID, sizeof(SID));
        offset += sizeof(SID);
        
        memcpy(buf + offset, &SIZE_BYTES, sizeof(SIZE_BYTES));
        offset += sizeof(SIZE_BYTES);
        
        // placeholder for delta time
        offset += RDINMEABINARYTIMESIZE;

        memcpy(buf + offset, &szHeader[0], sizeof(szHeader));
        offset += sizeof(szHeader);

        memcpy(buf + offset, &fCOGTrue, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &tcTrueIndicator, sizeof(char));
        offset += sizeof(char);

        memcpy(buf + offset, &fCOGMag, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &tcMagnIndicator, sizeof(char));
        offset += sizeof(char);

        memcpy(buf + offset, &fSpdOverGroundKts, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &tcKtsIndicator, sizeof(char));
        offset += sizeof(char);
        
        memcpy(buf + offset, &fSpdOverGroundKmh, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &tcKmhIndicator, sizeof(char));
        offset += sizeof(char);
        
        memcpy(buf + offset, &tcModeIndicator, sizeof(char));
        offset += sizeof(char);
        
        return offset;    
}

int RDINMEAVTGBinary::deserialise(const char* buf, const int bytes)
{
        int offset = 0;

       // serialise the binary header
        //memcpy(&RDINMEABinaryID, buf + offset, sizeof(RDINMEABinaryID));
        offset += sizeof(RDINMEABinaryID);
        
        //memcpy(&SID, buf + offset, sizeof(SID));
        offset += sizeof(SID);
        
        //memcpy(&SIZE_BYTES, buf + offset, sizeof(SIZE_BYTES));
        offset += sizeof(SIZE_BYTES);
        
        // placeholder for delta time
        offset += RDINMEABINARYTIMESIZE;

        memcpy(&szHeader[0], buf + offset, sizeof(szHeader));
        offset += sizeof(szHeader);

        memcpy(&fCOGTrue, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&tcTrueIndicator, buf + offset, sizeof(char));
        offset += sizeof(char);

        memcpy(&fCOGMag, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&tcMagnIndicator, buf + offset, sizeof(char));
        offset += sizeof(char);

        memcpy(&fSpdOverGroundKts, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&tcKtsIndicator, buf + offset, sizeof(char));
        offset += sizeof(char);
        
        memcpy(&fSpdOverGroundKmh, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&tcKmhIndicator, buf + offset, sizeof(char));
        offset += sizeof(char);
        
        memcpy(&tcModeIndicator, buf + offset, sizeof(char));
        offset += sizeof(char);
        
        return (offset > bytes ? -1 : offset);
}

RDINMEAVTGString::RDINMEAVTGString()
{
}

RDINMEAVTGString::RDINMEAVTGString(const RDINMEAVTGString& msg)
{
    
}

RDINMEAVTGString::~RDINMEAVTGString()
{

}

size_t RDINMEAVTGString::calc_length() const
{
        int size = SIZE_BYTES;        

        return size;            
}

int RDINMEAVTGString::serialise(char* buf, const int bytes) const
{
        int offset = 0;
    
        memcpy(buf + offset, &IdCode, sizeof(uint16_t));
        offset += sizeof(uint16_t);
        
        return SIZE_BYTES;
}

int RDINMEAVTGString::deserialise(const char* buf, const int bytes)
{
        int offset = 14;

        // for now, do this:
        memcpy(&VTGString[0], buf + offset, sizeof(VTGString));
        offset += sizeof(VTGString);
               
        return (offset > bytes ? -1 : offset);
}

RDINMEADBTBinary::RDINMEADBTBinary()
{
}


RDINMEADBTBinary::RDINMEADBTBinary(const RDINMEADBTBinary& msg)
{
    
}

RDINMEADBTBinary::~RDINMEADBTBinary()
{

}

size_t RDINMEADBTBinary::calc_length() const
{
        int size = SIZE_BYTES;        

        return size;            
}

int RDINMEADBTBinary::serialise(char* buf, const int bytes) const
{
        int offset = 0;

        // serialise the binary header
        memcpy(buf + offset, &RDINMEABinaryID, sizeof(RDINMEABinaryID));
        offset += sizeof(RDINMEABinaryID);
        
        memcpy(buf + offset, &SID, sizeof(SID));
        offset += sizeof(SID);
        
        memcpy(buf + offset, &SIZE_BYTES, sizeof(SIZE_BYTES));
        offset += sizeof(SIZE_BYTES);
        
        // placeholder for delta time
        offset += RDINMEABINARYTIMESIZE;

        memcpy(buf + offset, &szHeader[0], sizeof(szHeader));
        offset += sizeof(szHeader);

        memcpy(buf + offset, &fWaterDepth_ft, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &tcFeetIndicator, sizeof(char));
        offset += sizeof(char);

        memcpy(buf + offset, &fWaterDepth_m, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &tcMeterIndicator, sizeof(char));
        offset += sizeof(char);

        memcpy(buf + offset, &fWaterDepth_F, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &tcFathomIndicator, sizeof(char));
        offset += sizeof(char);
        
        return (offset > bytes ? -1 : offset);
}

int RDINMEADBTBinary::deserialise(const char* buf, const int bytes)
{
        int offset = 0;

       // serialise the binary header
        //memcpy(&RDINMEABinaryID, buf + offset, sizeof(RDINMEABinaryID));
        offset += sizeof(RDINMEABinaryID);
        
        //memcpy(&SID, buf + offset, sizeof(SID));
        offset += sizeof(SID);
        
        //memcpy(&SIZE_BYTES, buf + offset, sizeof(SIZE_BYTES));
        offset += sizeof(SIZE_BYTES);
        
        // placeholder for delta time
        offset += RDINMEABINARYTIMESIZE;

        memcpy(&szHeader[0], buf + offset, sizeof(szHeader));
        offset += sizeof(szHeader);

        memcpy(&fWaterDepth_ft, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&tcFeetIndicator, buf + offset, sizeof(char));
        offset += sizeof(char);

        memcpy(&fWaterDepth_m, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&tcMeterIndicator, buf + offset, sizeof(char));
        offset += sizeof(char);

        memcpy(&fWaterDepth_F, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&tcFathomIndicator, buf + offset, sizeof(char));
        offset += sizeof(char);
        
        return (offset > bytes ? -1 : offset);
}

RDINMEADBTString::RDINMEADBTString()
{
}

RDINMEADBTString::RDINMEADBTString(const RDINMEADBTString& msg)
{
    
}

RDINMEADBTString::~RDINMEADBTString()
{

}

size_t RDINMEADBTString::calc_length() const
{
        int size = SIZE_BYTES;        

        return size;            
}

int RDINMEADBTString::serialise(char* buf, const int bytes) const
{
        int offset = 0;
    
        memcpy(buf + offset, &IdCode, sizeof(uint16_t));
        offset += sizeof(uint16_t);
        
        return SIZE_BYTES;
}

int RDINMEADBTString::deserialise(const char* buf, const int bytes)
{
        int offset = 14;

        // for now, do this:
        memcpy(&DBTString[0], buf + offset, sizeof(DBTString));
        offset += sizeof(DBTString);
        
        return (offset > bytes ? -1 : offset);
}


RDINMEAHDTBinary::RDINMEAHDTBinary()
{
}

RDINMEAHDTBinary::RDINMEAHDTBinary(const RDINMEAHDTBinary& msg)
{
    
}

RDINMEAHDTBinary::~RDINMEAHDTBinary()
{

}

size_t RDINMEAHDTBinary::calc_length() const
{
        int size = SIZE_BYTES;        

        return size;            
}

int RDINMEAHDTBinary::serialise(char* buf, const int bytes) const
{
        int offset = 0;

        // serialise the binary header
        memcpy(buf + offset, &RDINMEABinaryID, sizeof(RDINMEABinaryID));
        offset += sizeof(RDINMEABinaryID);
        
        memcpy(buf + offset, &SID, sizeof(SID));
        offset += sizeof(SID);
        
        memcpy(buf + offset, &SIZE_BYTES, sizeof(SIZE_BYTES));
        offset += sizeof(SIZE_BYTES);
        
        // placeholder for delta time
        offset += RDINMEABINARYTIMESIZE;

        memcpy(buf + offset, &szHeader[0], sizeof(szHeader));
        offset += sizeof(szHeader);

        memcpy(buf + offset, &dHeading, sizeof(double));
        offset += sizeof(double);

        memcpy(buf + offset, &tcHeadingIndicator, sizeof(char));
        offset += sizeof(char);
        
        return (offset > bytes ? -1 : offset);
}

int RDINMEAHDTBinary::deserialise(const char* buf, const int bytes)
{
        int offset = 0;

       // serialise the binary header
        //memcpy(&RDINMEABinaryID, buf + offset, sizeof(RDINMEABinaryID));
        offset += sizeof(RDINMEABinaryID);
        
        //memcpy(&SID, buf + offset, sizeof(SID));
        offset += sizeof(SID);
        
        //memcpy(&SIZE_BYTES, buf + offset, sizeof(SIZE_BYTES));
        offset += sizeof(SIZE_BYTES);
        
        // placeholder for delta time
        offset += RDINMEABINARYTIMESIZE;

        memcpy(&szHeader[0], buf + offset, sizeof(szHeader));
        offset += sizeof(szHeader);

        memcpy(&dHeading, buf + offset, sizeof(double));
        offset += sizeof(double);

        memcpy(&tcHeadingIndicator, buf + offset, sizeof(char));
        offset += sizeof(char);
        
        return (offset > bytes ? -1 : offset);
}

RDINMEAHDTString::RDINMEAHDTString()
{
}


RDINMEAHDTString::RDINMEAHDTString(const RDINMEAHDTString& msg)
{
    
}

RDINMEAHDTString::~RDINMEAHDTString()
{

}

size_t RDINMEAHDTString::calc_length() const
{
        int size = SIZE_BYTES;        

        return size;            
}

int RDINMEAHDTString::serialise(char* buf, const int bytes) const
{
        int offset = 0;
    
        memcpy(buf + offset, &IdCode, sizeof(uint16_t));
        offset += sizeof(uint16_t);
        
        return SIZE_BYTES;
}

int RDINMEAHDTString::deserialise(const char* buf, const int bytes)
{
        int offset = 14;

        std::string s(buf + offset);
        
        // for now, do this:
        memcpy(&HDTString[0], buf + offset, sizeof(HDTString));
        offset += sizeof(HDTString);
        
        return (offset > bytes ? -1 : offset);
}

SONSystemConfig::SONSystemConfig() :
        TypeNumber(0),
        SubTypeField(0),
        SequenceNumber(0),
        DSPFirmwareVersion(0),
        ARMFirmwareVersion(0),
        FPGAFirmwareVersion(0),
        WebServerFirmwareVersion(0),
        OffsetXGPS(0),
        OffsetYGPS(0),
        OffsetZGPS(0),
        PitchOffsetDegrees(0),
        RollOffsetDegrees(0),
        HeadingOffsetDegrees(0)
{
        memset(&SerialNumber[0], 0x0, SERIAL_NUMBER_BYTES);
}

SONSystemConfig::SONSystemConfig(const SONSystemConfig& msg) :
        TypeNumber(msg.TypeNumber),
        SubTypeField(msg.SubTypeField),
        SequenceNumber(msg.SequenceNumber),
        DSPFirmwareVersion(msg.DSPFirmwareVersion),
        ARMFirmwareVersion(msg.ARMFirmwareVersion),
        FPGAFirmwareVersion(msg.FPGAFirmwareVersion),
        WebServerFirmwareVersion(msg.WebServerFirmwareVersion),
        OffsetXGPS(msg.OffsetXGPS),
        OffsetYGPS(msg.OffsetYGPS),
        OffsetZGPS(msg.OffsetZGPS),
        PitchOffsetDegrees(msg.PitchOffsetDegrees),
        RollOffsetDegrees(msg.RollOffsetDegrees),
        HeadingOffsetDegrees(msg.HeadingOffsetDegrees)
{
        memcpy(&SerialNumber[0], &msg.SerialNumber[0], SERIAL_NUMBER_BYTES);    
}

SONSystemConfig::~SONSystemConfig()
{

}

size_t SONSystemConfig::calc_length() const
{
        return SIZE_BYTES;       
}

int SONSystemConfig::serialise(char* buf, const int bytes) const
{
        int offset = 0;

        memcpy(buf + offset, &IdCode, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &TypeNumber, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &SubTypeField, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &SerialNumber[0], SERIAL_NUMBER_BYTES*sizeof(uint8_t));
        offset += SERIAL_NUMBER_BYTES*sizeof(uint8_t);

        memcpy(buf + offset, &SequenceNumber, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &DSPFirmwareVersion, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(buf + offset, &ARMFirmwareVersion, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(buf + offset, &FPGAFirmwareVersion, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(buf + offset, &WebServerFirmwareVersion, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(buf + offset, &OffsetXGPS, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &OffsetYGPS, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &OffsetZGPS, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &PitchOffsetDegrees, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &RollOffsetDegrees, sizeof(float));
        offset += sizeof(float);

        memcpy(buf + offset, &HeadingOffsetDegrees, sizeof(float));
        offset += sizeof(float);

        return offset;
}

int SONSystemConfig::deserialise(const char* buf, const int bytes)
{
        int offset = 0;

        //memcpy(&IdCode, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&TypeNumber, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&SubTypeField, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&SerialNumber[0], buf + offset, SERIAL_NUMBER_BYTES*sizeof(uint8_t));
        offset += SERIAL_NUMBER_BYTES*sizeof(uint8_t);

        memcpy(&SequenceNumber, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&DSPFirmwareVersion, buf + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(&ARMFirmwareVersion, buf + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(&FPGAFirmwareVersion, buf + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(&WebServerFirmwareVersion, buf + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(&OffsetXGPS, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&OffsetYGPS, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&OffsetZGPS, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&PitchOffsetDegrees, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&RollOffsetDegrees, buf + offset, sizeof(float));
        offset += sizeof(float);

        memcpy(&HeadingOffsetDegrees, buf + offset, sizeof(float));
        offset += sizeof(float);

        return offset;
}

SONVariableLeader::SONVariableLeader() :
        Status(0),
        PingId(0),
        Waveform(0),
        RxGain(0),
        TxGain(0),
        INSBeam0VelMms(0),
        INSBeam1VelMms(0),
        INSBeam2VelMms(0),
        INSBeam3VelMms(0),
        INSHeading(0),
        INSPitch(0),
        INSRoll(0),
        Reserved(0)
{
        memset(&T0Time[0], 0x0, T0TIME_BYTES);    
}

SONVariableLeader::SONVariableLeader(const SONVariableLeader& msg) :
        Status(msg.Status),
        PingId(msg.PingId),
        Waveform(msg.Waveform),
        RxGain(msg.RxGain),
        TxGain(msg.TxGain),
        INSBeam0VelMms(msg.INSBeam0VelMms),
        INSBeam1VelMms(msg.INSBeam1VelMms),
        INSBeam2VelMms(msg.INSBeam2VelMms),
        INSBeam3VelMms(msg.INSBeam3VelMms),
        INSHeading(msg.INSHeading),
        INSPitch(msg.INSPitch),
        INSRoll(msg.INSRoll),
        Reserved(msg.Reserved)
{
        memcpy(&T0Time[0], &msg.T0Time[0], T0TIME_BYTES);    
}

SONVariableLeader::~SONVariableLeader()
{

}

size_t SONVariableLeader::calc_length() const
{
        return SIZE_BYTES;
}

int SONVariableLeader::serialise(char* buf, const int bytes) const
{
        int offset = 0;

        memcpy(buf + offset, &IdCode, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &Status, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &PingId, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &T0Time[0], T0TIME_BYTES*sizeof(uint8_t));
        offset += T0TIME_BYTES*sizeof(uint8_t);

        memcpy(buf + offset, &Waveform, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(buf + offset, &RxGain, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &TxGain, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &SensorValidity, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &INSBeam0VelMms, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(buf + offset, &INSBeam1VelMms, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(buf + offset, &INSBeam2VelMms, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(buf + offset, &INSBeam3VelMms, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(buf + offset, &INSHeading, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &INSPitch, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(buf + offset, &INSRoll, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(buf + offset, &BeamAmbLimitMms, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(buf + offset, &BeamVelRmsMms, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(buf + offset, &Reserved, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        return offset;
}

int SONVariableLeader::deserialise(const char* buf, const int bytes)
{
        int offset = 0;

        //memcpy(&IdCode, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&Status, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&PingId, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&T0Time[0], buf + offset, T0TIME_BYTES*sizeof(uint8_t));
        offset += T0TIME_BYTES*sizeof(uint8_t);

        memcpy(&Waveform, buf + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        memcpy(&RxGain, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&TxGain, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&SensorValidity, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&INSBeam0VelMms, buf + offset, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(&INSBeam1VelMms, buf + offset, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(&INSBeam2VelMms, buf + offset, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(&INSBeam3VelMms, buf + offset, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(&INSHeading, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&INSPitch, buf + offset, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(&INSRoll, buf + offset, sizeof(int16_t));
        offset += sizeof(int16_t);

        memcpy(&BeamAmbLimitMms, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        memcpy(&BeamVelRmsMms, buf + offset, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        memcpy(&Reserved, buf + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        return offset;
}

SONOriginSystemLeader::SONOriginSystemLeader() :
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
		u32_EventStartTimeS(0),
		u32_EventStartTime_us(0),
		u64_UnixEpochTime_ms(0),
		u32_CarrierFrequencyHz(0),
		u32_RxBandwidthHz(0),
		u32_TxBandwidthHz(0),
		u16_RxGaindB(0),
		u16_TxLevel(0),
		u16_SIG_BaseCounts(0),
		u16_SIG_BT(0)
{
        
}

SONOriginSystemLeader::SONOriginSystemLeader(const SONOriginSystemLeader& msg) :
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
		u32_EventStartTimeS(msg.u32_EventStartTimeS),
		u32_EventStartTime_us(msg.u32_EventStartTime_us),
		u64_UnixEpochTime_ms(msg.u64_UnixEpochTime_ms),
		u32_CarrierFrequencyHz(msg.u32_CarrierFrequencyHz),
		u32_RxBandwidthHz(msg.u32_RxBandwidthHz),
		u32_TxBandwidthHz(msg.u32_TxBandwidthHz),
		u16_RxGaindB(msg.u16_RxGaindB),
		u16_TxLevel(msg.u16_TxLevel),
		u16_SIG_BaseCounts(msg.u16_SIG_BaseCounts),
		u16_SIG_BT(msg.u16_SIG_BT)
{
        
}

SONOriginSystemLeader::~SONOriginSystemLeader()
{

}

size_t SONOriginSystemLeader::calc_length() const
{
    return SIZE_BYTES;
}

int SONOriginSystemLeader::serialise(char* buf, const int bytes) const
{    
    int offset = 0;
    
    memcpy(buf + offset, &IdCode, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &u32_SerialNum, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(buf + offset, &u16_SubSerialNum, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &u16_TypeNum, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &u16_SubTypeNum, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &u16_BeamControl, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &u16_EventID, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &u16_PingID, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &u16_BurstID, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &u16_CaptureDurationS, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &u32_SleepDurationS, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(buf + offset, &u16_PingFrequency_mHz, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &u16_BeamIndex, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &u32_PingCounter, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(buf + offset, &u16_NoBeams, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &u32_EventStartTimeS, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(buf + offset, &u32_EventStartTime_us, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(buf + offset, &u64_UnixEpochTime_ms, sizeof(uint64_t));
    offset += sizeof(uint64_t);

    memcpy(buf + offset, &u32_CarrierFrequencyHz, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(buf + offset, &u32_RxBandwidthHz, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(buf + offset, &u32_TxBandwidthHz, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(buf + offset, &u16_RxGaindB, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &u16_TxLevel, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &u16_SIG_BaseCounts, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &u16_SIG_BT, sizeof(uint16_t));
    offset += sizeof(uint16_t);
    return offset;
}

int SONOriginSystemLeader::deserialise(const char* buf, const int bytes)
{
    int offset = 0;
    
    //memcpy(&IdCode, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&u32_SerialNum, buf + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(&u16_SubSerialNum, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&u16_TypeNum, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&u16_SubTypeNum, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&u16_BeamControl, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&u16_EventID, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&u16_PingID, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&u16_BurstID, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&u16_CaptureDurationS, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&u32_SleepDurationS, buf + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(&u16_PingFrequency_mHz, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&u16_BeamIndex, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&u32_PingCounter, buf + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(&u16_NoBeams, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&u32_EventStartTimeS, buf + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(&u32_EventStartTime_us, buf + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(&u64_UnixEpochTime_ms, buf + offset, sizeof(uint64_t));
    offset += sizeof(uint64_t);

    memcpy(&u32_CarrierFrequencyHz, buf + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(&u32_RxBandwidthHz, buf + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(&u32_TxBandwidthHz, buf + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(&u16_RxGaindB, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&u16_TxLevel, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&u16_SIG_BaseCounts, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&u16_SIG_BT, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    return offset;
}

SONPIESVariableLeader::SONPIESVariableLeader() :
        SV1_ms(0),
        SV2_ms(0), 
        TOF1_s(0), 
        TOF2_s(0),
        Magnitude(0),
        PeakIndex(0),
        RMLEIndex(0),
        RMLERange(0),
        PretriggerRMS(0),
        HalfLife_s(0),     
        R2(0),           
        Pressure_kPa(0),     
        Depth_m(0),        
        Zoffset_m(0),      
        Latitude_DD(0),     
        Patmos_kPa(0),
        TimeNS(0)
{
        
}

SONPIESVariableLeader::SONPIESVariableLeader(const SONPIESVariableLeader& msg) :
        SV1_ms(msg.SV1_ms),
        SV2_ms(msg.SV2_ms), 
        TOF1_s(msg.TOF1_s), 
        TOF2_s(msg.TOF2_s),
        Magnitude(msg.Magnitude),
        PeakIndex(msg.PeakIndex),
        RMLEIndex(msg.RMLEIndex),
        RMLERange(msg.RMLERange),
        PretriggerRMS(msg.PretriggerRMS),
        HalfLife_s(msg.HalfLife_s),     
        R2(msg.R2),           
        Pressure_kPa(msg.Pressure_kPa),     
        Depth_m(msg.Depth_m),        
        Zoffset_m(msg.Zoffset_m),      
        Latitude_DD(msg.Latitude_DD),     
        Patmos_kPa(msg.Patmos_kPa),
        TimeNS(msg.TimeNS)
{
        
}

SONPIESVariableLeader::~SONPIESVariableLeader()
{

}

size_t SONPIESVariableLeader::calc_length() const
{
    return SIZE_BYTES;
}

int SONPIESVariableLeader::serialise(char* buf, const int bytes) const
{    
    int offset = 0;
    
    memcpy(buf + offset, &IdCode, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(buf + offset, &SV1_ms, sizeof(float));
    offset += sizeof(float);

    memcpy(buf + offset, &SV2_ms, sizeof(float));
    offset += sizeof(float);

    memcpy(buf + offset, &TOF1_s, sizeof(float));
    offset += sizeof(float);

    memcpy(buf + offset, &TOF2_s, sizeof(float));
    offset += sizeof(float);

    memcpy(buf + offset, &Magnitude, sizeof(float));
    offset += sizeof(float);

    memcpy(buf + offset, &PeakIndex, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(buf + offset, &RMLEIndex, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(buf + offset, &RMLERange, sizeof(float));
    offset += sizeof(float);

    memcpy(buf + offset, &PretriggerRMS, sizeof(float));
    offset += sizeof(float);

    memcpy(buf + offset, &HalfLife_s, sizeof(float));
    offset += sizeof(float);

    memcpy(buf + offset, &R2, sizeof(float));
    offset += sizeof(float);

    memcpy(buf + offset, &Pressure_kPa, sizeof(float));
    offset += sizeof(float);

    memcpy(buf + offset, &Depth_m, sizeof(float));
    offset += sizeof(float);

    memcpy(buf + offset, &Zoffset_m, sizeof(float));
    offset += sizeof(float);

    memcpy(buf + offset, &Latitude_DD, sizeof(float));
    offset += sizeof(float);

    memcpy(buf + offset, &Patmos_kPa, sizeof(float));
    offset += sizeof(float);

    memcpy(buf + offset, &TimeNS, sizeof(uint64_t));
    offset += sizeof(uint64_t);

    return offset;
}

int SONPIESVariableLeader::deserialise(const char* buf, const int bytes)
{
    int offset = 0;
    
    //memcpy(&IdCode, buf + offset, sizeof(uint16_t));
    offset += sizeof(uint16_t);

    memcpy(&SV1_ms, buf + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&SV2_ms, buf + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&TOF1_s, buf + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&TOF2_s, buf + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&Magnitude, buf + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&PeakIndex, buf + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(&RMLEIndex, buf + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    memcpy(&RMLERange, buf + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&PretriggerRMS, buf + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&HalfLife_s, buf + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&R2, buf + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&Pressure_kPa, buf + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&Depth_m, buf + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&Zoffset_m, buf + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&Latitude_DD, buf + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&Patmos_kPa, buf + offset, sizeof(float));
    offset += sizeof(float);

    memcpy(&TimeNS, buf + offset, sizeof(uint64_t));
    offset += sizeof(uint64_t);

    return offset;
}



PD0Message::PD0Message()
{
    add_section(new RDIPD0Header());
}

//copy constructor
PD0Message::PD0Message(const PD0Message& orig):
    Checksum(orig.Checksum),
    Reserved(orig.Reserved),
    BaseFrame(orig.BaseFrame)
{
    //need to populate ma_Fields structure
    clone_all_sections(orig);
}

//copy assignment - do we need this?
PD0Message& PD0Message::operator=(const PD0Message& orig)
{
    Checksum = orig.Checksum;
    Reserved = orig.Reserved;
    BaseFrame = orig.BaseFrame;
    if(&orig != this) {
        delete_all_sections();
        clone_all_sections(orig);
    }
    return *this;
}

PD0Message::~PD0Message()
{
    delete_all_sections();
}

void PD0Message::delete_all_sections()
{
    for (fields_it_t it = ma_Fields.begin(); it != ma_Fields.end(); ++it)
    {
        if (it->second)
        {
            delete it->second;
            it->second = NULL;
        }
    }
}

void PD0Message::clone_all_sections(const PD0Message& orig)
{
    for (auto it = orig.ma_Fields.begin(); it != orig.ma_Fields.end(); ++it) {
        if (it->second) {
            ma_Fields[it->first] = (it->second)->clone();
        }
    }
}

bool PD0Message::section_present(const uint16_t& Id)
{
        return (ma_Fields.find(Id) != ma_Fields.end()); 
}

void PD0Message::add_section(RDIPD0Interface* pd0_section)
{
        // don't insert NULL sections
        if (pd0_section == NULL)
        {
            printf("%s Exit due to NULL section\n", __PRETTY_FUNCTION__);
            return;
        }
        
        // don't insert a section if the header isn't present
        if (!section_present(RDIPD0Header::Id) && pd0_section->get_ID() != RDIPD0Header::Id)
        {
            printf("%s Exit due to no header section\n", __PRETTY_FUNCTION__);
            return;
        }

        uint16_t pd0_section_id = pd0_section->get_ID();

        if (ma_Fields[pd0_section_id])
        {
            delete ma_Fields[pd0_section_id];
            ma_Fields.erase(pd0_section_id);
        }

        ma_Fields[pd0_section_id] = pd0_section;
}

RDIPD0Interface* PD0Message::get_section(const uint16_t& id) const
{
    if (ma_Fields.find(id) != ma_Fields.end())
    {
        return ma_Fields.at(id);
    }
    else
    {
        return NULL;
    }
}

size_t PD0Message::calc_length() const
{
        // update the header        
        RDIPD0Header* hdr    = (RDIPD0Header*) ma_Fields.at(RDIPD0Header::Id);
        hdr->Indices         = calc_indices();
        hdr->NumberDataTypes = ma_Fields.size() - 1; 
        
        size_t length_bytes = 0;

        for (fields_cit_t it = ma_Fields.begin(); it != ma_Fields.end(); ++it)
        {
            if (it->second)
            {
                length_bytes += it->second->calc_length();
            }
        }

        // add reserved
        length_bytes += RESERVED_SIZE_BYTES;

        // add checksum
        length_bytes += CHECKSUM_SIZE_BYTES;

        hdr->EnsembleBytes = length_bytes - CHECKSUM_SIZE_BYTES;
        
        return length_bytes;
}

uint16_t PD0Message::calc_checksum(const char* buf, const int& buf_size_bytes) const
{
        uint16_t checksum = 0;

        for (int k = 0; k < buf_size_bytes; ++k)
        {
            checksum += *((uint8_t*) (buf + k));
        }

        return checksum;
}

std::vector<uint16_t> PD0Message::calc_indices() const
{
        std::vector<uint16_t> indices;

        uint16_t offset = RDIPD0Header::SIZE_BYTES + sizeof(uint16_t) * (ma_Fields.size() - 1);

        for (fields_cit_t it = ma_Fields.begin(); it != ma_Fields.end(); ++it)
        {
            if (it->second && it->first != RDIPD0Header::Id)
            {
                uint16_t length = it->second->calc_length();

                indices.push_back(offset);

                offset += length;
            }
        }

        return indices;
}

int PD0Message::serialise(char* buf, const int bytes) const
{
        if (ma_Fields.find(RDIPD0Header::Id) == ma_Fields.end())
        {
            return -1;
        }

        RDIPD0Header* hdr = (RDIPD0Header*) ma_Fields.at(RDIPD0Header::Id);

        if (hdr == NULL)
        {
            return -1;
        }

        // update the header        
        hdr->EnsembleBytes   = calc_length() - CHECKSUM_SIZE_BYTES ;
        hdr->Indices         = calc_indices();
        hdr->NumberDataTypes = ma_Fields.size() - 1; 
        
        //printf("%s %ld %hu %hhu\n", __PRETTY_FUNCTION__, hdr->Indices.size(), hdr->EnsembleBytes, hdr->NumberDataTypes);

        int offset = hdr->serialise(buf, bytes);

        //printf("%s 0x%04x %d %d\n", __PRETTY_FUNCTION__, hdr->get_ID(), offset, 0);
        
        for (fields_cit_t it = ma_Fields.begin(); it != ma_Fields.end(); ++it)
        {
            if (it->second && it->first != RDIPD0Header::Id)
            {
                int bytes_serialised = it->second->serialise(buf + offset, bytes);
                
                //printf("%s 0x%04x %d %d\n", __PRETTY_FUNCTION__, it->second->get_ID(), bytes_serialised, offset);        
                                
                offset += bytes_serialised;
            }
        }

        // reserved
        memcpy(buf + offset, &Reserved, sizeof(Reserved));
        offset += sizeof(Reserved);

        // calculate checksum
        uint16_t checksum = calc_checksum(buf, bytes);

        memcpy(buf + offset, &checksum, sizeof(checksum));
        offset += sizeof(checksum);

        return offset;    
}

EFrame PD0Message::byte_to_eframe(uint8_t n)
{
    uint8_t masked_and_shifted = (n >> 3) & 3;
    if (masked_and_shifted == 0)
    return EFrame::BEAM;

    if (masked_and_shifted == 1)
    return EFrame::INSTRUMENT;

    if (masked_and_shifted == 2)
    return EFrame::VESSEL;

    if (masked_and_shifted == 3)
    return EFrame::EARTH;

    return EFrame::EARTH;
}

int PD0Message::deserialise(const char* buf, const int bytes)
{
        int offset = 0;    

        if (ma_Fields.find(RDIPD0Header::Id) == ma_Fields.end() ||
            ma_Fields.at(RDIPD0Header::Id)   == NULL)
        {
            // no header set
            printf("%s Exit due to no header section\n", __PRETTY_FUNCTION__);
            return -1;
        }

        RDIPD0Header* hdr = (RDIPD0Header*) ma_Fields.at(RDIPD0Header::Id);

        if (hdr)
        {
                int ret = hdr->deserialise(buf, bytes);
                if(ret == -1)
                {
                    printf("%s Exit due to invalid header section\n", __PRETTY_FUNCTION__);
                    return -1;
                }
                offset += ret;
                // create a local copy of the indices - this allows the add_section function
                // to be used on a pre-existing ensemble
                std::vector<uint16_t> Indices = hdr->Indices;
                
                for (size_t k = 0; k < Indices.size(); ++k)
                {
                        // get code - basic check we can deref...
                        if(Indices[k] >= bytes)
                        {
                            //corrupt file - bad indices
                            printf("%s Exit due to invalid index\n", __PRETTY_FUNCTION__);
                            return -1;
                        }
                        uint16_t code = *((uint16_t*) (buf + Indices[k]));

                        RDIPD0Interface* section = NULL;

                        bool standard_section = true;
                        
                        // Look for codes here
                        if (code == RDIFixedLeader::Id) // fixed leader
                        {
                            section = new RDIFixedLeader();
                        }
                        else if (code == RDIVariableLeader::Id) // variable leader
                        {
                            section = new RDIVariableLeader();
                        }
                        else if (code == RDIVelocity::Id && section_present(RDIFixedLeader::Id)) // velocity data 
                        {
                            RDIFixedLeader* fixed_leader = (RDIFixedLeader*) ma_Fields.at(RDIFixedLeader::Id);
                            section = new RDIVelocity(fixed_leader->NumberOfCells);
                        }
                        else if (code == RDIXC::Id && section_present(RDIFixedLeader::Id)) // XC magnitude data 
                        {
                            RDIFixedLeader* fixed_leader = (RDIFixedLeader*) ma_Fields.at(RDIFixedLeader::Id);
                            section = new RDIXC(fixed_leader->NumberOfCells);
                        }
                        else if (code == RDIIntensity::Id && section_present(RDIFixedLeader::Id)) // Echo Intensity data 
                        {
                            RDIFixedLeader* fixed_leader = (RDIFixedLeader*) ma_Fields.at(RDIFixedLeader::Id);
                            section = new RDIIntensity(fixed_leader->NumberOfCells);
                        }
                        else if (code == RDIPrctGood::Id && section_present(RDIFixedLeader::Id)) // percentage good data 
                        {
                            RDIFixedLeader* fixed_leader = (RDIFixedLeader*) ma_Fields.at(RDIFixedLeader::Id);
                            section = new RDIPrctGood(fixed_leader->NumberOfCells);
                        }
                        else if (code == RDIVerticalBeamLeader::Id)
                        {
                            section = new RDIVerticalBeamLeader();
                        }
                        else if (code == RDIVerticalBeamVelocity::Id && section_present(RDIFixedLeader::Id))
                        {
                            RDIFixedLeader* fixed_leader = (RDIFixedLeader*) ma_Fields.at(RDIFixedLeader::Id);
                            section = new RDIVerticalBeamVelocity(fixed_leader->NumberOfCells);
                        }
                        else if (code == RDIVerticalBeamXC::Id && section_present(RDIFixedLeader::Id))
                        {
                            RDIFixedLeader* fixed_leader = (RDIFixedLeader*) ma_Fields.at(RDIFixedLeader::Id);
                            section = new RDIVerticalBeamXC(fixed_leader->NumberOfCells);
                        }
                        else if (code == RDIVerticalBeamIntensity::Id && section_present(RDIFixedLeader::Id))
                        {
                            RDIFixedLeader* fixed_leader = (RDIFixedLeader*) ma_Fields.at(RDIFixedLeader::Id);
                            section = new RDIVerticalBeamIntensity(fixed_leader->NumberOfCells);
                        }
                        else if (code == RDIVerticalBeamPrctGood::Id && section_present(RDIFixedLeader::Id))
                        {
                            RDIFixedLeader* fixed_leader = (RDIFixedLeader*) ma_Fields.at(RDIFixedLeader::Id);
                            section = new RDIVerticalBeamPrctGood(fixed_leader->NumberOfCells);
                        }
                        else if (code == RDIBottomTrack::Id) // bottom track data 
                        {
                            section = new RDIBottomTrack();
                        }
                        else if (code == RDINMEABinaryID) // RDI V2 General header
                        {
                            // get SID
                            if(Indices[k] + sizeof(uint16_t) >= bytes)
                            {
                                //corrupt file - bad indices
                                printf("%s Exit due to invalid section\n", __PRETTY_FUNCTION__);
                                return -1;
                            }
                            uint16_t SID = *((uint16_t*) (buf + Indices[k] + sizeof(uint16_t)));

                            if (SID == RDINMEAGGABinary::SID)
                            {
                                section = new RDINMEAGGABinary();
                            }
                            else if (SID == RDINMEAVTGBinary::SID)
                            {
                                section = new RDINMEAVTGBinary();
                            }
                            else if (SID == RDINMEADBTBinary::SID)
                            {
                                section = new RDINMEADBTBinary();
                            }
                            else if (SID == RDINMEAHDTBinary::SID)
                            {
                                section = new RDINMEAHDTBinary();
                            }
                            else
                            {
                                // not recognised; skip
                            }                    
                        }
                        else if (code == 0x2100) // DBT String
                        {
                            section = new RDINMEADBTString();
                        }
                        else if (code == 0x2101) // GGA string
                        {
                            section = new RDINMEAGGAString();
                        }
                        else if (code == 0x2102) // VTG string
                        {
                            section = new RDINMEAVTGString();
                        }
                        else if (code == 0x2104) // HDT string
                        {
                            section = new RDINMEAHDTString();
                        }
                        else if (code == SONSystemConfig::Id) // sonardyne system config
                        {
                            section = new SONSystemConfig();
                        }
                        else if (code == SONVariableLeader::Id) // sonardyne variable leader
                        {
                            section = new SONVariableLeader();
                        }        
                        else if (code == SONOriginSystemLeader::Id) // Origin System leader
                        {
                            section = new SONOriginSystemLeader();
                        }        
                        else if (code == SONPIESVariableLeader::Id) // PIES variable leader
                        {
                            section = new SONPIESVariableLeader();
                        }        
                        else
                        {
                            // non-conventional section; check if it has already been inserted
                            if (ma_Fields.find(code) != ma_Fields.end())
                            {
                                section = ma_Fields[code];
                                standard_section = false;
                            }
                        }
                        
                        if (section)
                        {
                                if(Indices[k] + section->calc_length() >= bytes)
                                {
                                    //corrupt file - bad indices
                                    printf("%s Exit due to invalid section size\n", __PRETTY_FUNCTION__);
                                    return -1;
                                }
                                int bytes_deserialised = section->deserialise(buf + Indices[k], section->calc_length());

                                offset += bytes_deserialised;
                                if (code == RDIFixedLeader::Id) // fixed leader
                                 {
                                    RDIFixedLeader*    pcFixedLeader     = (RDIFixedLeader*) section;
                                    BaseFrame = byte_to_eframe(pcFixedLeader->CoordTxfrm);
                                 }
                                if (standard_section)
                                {
                                    add_section(section);
                                }
                        }
                }

            memcpy(&Reserved, buf + offset, sizeof(Reserved));
            offset += RESERVED_SIZE_BYTES;

            // calculate checksum
            memcpy(&Checksum, buf + offset, sizeof(Checksum));
            //uint16_t checksum = calc_checksum(buf, offset);
			calc_checksum(buf, offset); // why is this here? should we compare?

            // compare

            offset += CHECKSUM_SIZE_BYTES;
        }
        
        return offset;
}

std::vector<uint16_t> PD0Message::get_all_fields_ids()
{
    std::vector<uint16_t> fields_ids;
	for (auto const& x : ma_Fields)
	{
		fields_ids.emplace_back(x.first);
	}
    return fields_ids;
}

void PD0Message::transform(const EFrame& eFrame)
{
        RDIFixedLeader*    pcFixedLeader     = (RDIFixedLeader*)    get_section(RDIFixedLeader::Id);
        RDIVariableLeader* pcVariableLeader  = (RDIVariableLeader*) get_section(RDIVariableLeader::Id);
        RDIVelocity*       pcVelocity        = (RDIVelocity*)       get_section(RDIVelocity::Id);

        if (pcFixedLeader == NULL || pcVariableLeader == NULL || pcVelocity == NULL)
        {
            return;
        }

        if (eFrame <= EFrame::EARTH)
        {
            // perform transformations
            pcVelocity->transform(eFrame, BaseFrame, pcFixedLeader, pcVariableLeader);
        }

        // set the frame in the fixed leader
        pcFixedLeader->CoordTxfrm = 0;
        
        if (eFrame == EFrame::BEAM)
        {
            ;
        }        
        else if (eFrame == EFrame::INSTRUMENT)
        {   
            pcFixedLeader->CoordTxfrm |= (1 << 3);
        }
        else if(eFrame == EFrame::VESSEL)
        {   
            pcFixedLeader->CoordTxfrm |= (1 << 4);
        }
        else if(eFrame == EFrame::EARTH)
        {        
            pcFixedLeader->CoordTxfrm |= (1 << 2); // we have used tilts in this transformation
            pcFixedLeader->CoordTxfrm |= (1 << 3);
            pcFixedLeader->CoordTxfrm |= (1 << 4);
        }
        
}

