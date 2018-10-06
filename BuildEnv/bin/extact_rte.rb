#!/usr/bin/env ruby

require 'FileUtils'

# CONST(Rte_CDS_SwcRdciComHdlA, RTE_CONST) Rte_Instance_SwcRdciComHdlA =
SWC_INSTANCE_START = /^CONST.+Rte_Instance_SwcRdci\w+\s*=/
SWC_INSTANCE_END   = /};/

# Rte_Send_SwcRdciComHdlA_RdciComHdlOutputA_RDC_TYR_ID1
RTE_FUNC_SEND_QUEUE_OUTPUT = /^FUNC.*Rte_Send_\w*RdciComHdl.*Output[A-Za-z0-9]+_([A-Z0-9_]+)\s*\((.*)/

#Rte_Write_TrwEspArIf_RdciWarnExtDscModulAufbertgRaddrehzhlkommu_IstAnzhlGebrflankRadVL
RTE_FUNC_WRITE_ABS_EDGE_START = /^FUNC.*Rte_Write_TrwEspArIf_RdciWarnExtDsc/

# Rte_Send_SwcRdciComHdlA_RdciEinleseinhRdcSensEinlesRdcSens_LfrntId
RTE_FUNC_SEND_QUEUE_START = /^FUNC.*Rte_Send_\w*Rdc.*\)\s*$/

# Rte_Call_SwcRdciDat_NvmGenericRdciBlock_WriteBlock
RTE_FUNC_NVM_WRITE_BLOCK = /^FUNC.*Rte_Call_SwcRdci.*_(Nvm\w+)_WriteBlock/

# Rte_Call_SwcRdciDat_NvmDiagWarnings_SetRamBlockStatus
RTE_FUNC_NVM_SET_RAM_STATUS = /^FUNC.*Rte_Call_SwcRdci.*_(Nvm\w+)_SetRamBlockStatus/

#Rte_Receive_SwcRdciWarn_RdciWarnExtEinleseinhRdcSensEinlesRdcSens_RdcDat
RTE_FUNC_RECEIVE_QUEUE_START = /^FUNC.*Rte_Receive_SwcRdc.*\)\s*$/

#FUNC(CRdciWarnDelay, RTE_CODE) Rte_CData_TrwEspArIf_CRdciWarnDelay
RTE_FUNC_CALPRM_CALL_START   = /^FUNC.*CRdci/

#Rte_Call_SwcRdciDat_DemErrorRdciWuDefectRl_SetEventStatus
RTE_FUNC_DEM_CALL_START   = /^FUNC.*Rte_Call_SwcRdci.*DemError([0-9A-Za-z]+)_/

# How to find the end of the function calls above!
FUNC_RETURN_POINT = /return.*;/


#Other stuff of interest
RTE_INCLUDE      = /#include.*Rte_SwcRdci/
RTE_PIM          = /^VAR.*Rte_Pim.*SwcRdc/
RTE_VAR_CALPRM   = /^VAR.*CRdci/
RTE_CCM_HDL      = /^FUNC.*Rte_Call_SwcRdciDat_IntCcHdl_(\w+)\s*\((.*)/

#Rte_DataHandleBuffer_2044.value = *((P2CONST(IstAnzhlGebrflankRadHLType, AUTOMATIC, BASE_APPL_DATA)) (P2CONST(void, AUTOMATIC, BASE_APPL_DATA)) (&Rte_BDS.Rte_ReceiveBuffer_1195.value));
RTE_DSC_EDGE_BUFFER_MAP = /\s*Rte_DataHandleBuffer_(\d+).value\s*=.*Rte_ReceiveBuffer_(\d+).value/


def puts_to_both( text )

    @c_file.puts text
    @h_file.puts text

end

def output_copyright( file )

    file.puts '/*! \file'
    file.puts '*               $Revision: 13759 $'
    file.puts '*'
    file.puts '*               $Author: amillen $'
    file.puts '*'
    file.puts '*               $Date: 2013-06-19 15:31:56 +0100 (Wed, 19 Jun 2013) $'
    file.puts '*'
    file.puts '*               $HeadURL: https://selacvs01.schrader.local:8443/svn/ECU_Software/BMW/trunk/TPMS_SWC/devel/SEL_RTE/SEL_RTE.c $'
    file.puts '*'
    file.puts '*   \brief      SEL RTE layer to allow running or Rdci in CANoe'
    file.puts '*/'
    file.puts '/* COPYRIGHT NOTICE'
    file.puts '* =================================================================================================='
    file.puts '*'
    file.puts '* The contents of this document are protected under copyright and contain commercially and/or'
    file.puts '* technically confidential information. The content of this document must not be used other than'
    file.puts '* for the purpose for which it was provided nor may it be disclosed or copied (by the authorised'
    file.puts '* recipient or otherwise) without the prior written consent of an authorised officer of Schrader'
    file.puts '* Electronics Ltd.'
    file.puts '*'
    file.puts '*         (C) $Date:: 2013#$ Schrader Electronics Ltd.'
    file.puts '*'
    file.puts '****************************************************************************************************'
    file.puts '*/'
    file.puts ''

end

def output_headers()
    output_copyright( @c_file )
    output_copyright( @h_file )

    @c_file.puts '/** This is the RTE core logic so we must define RTE_CORE before including anything! */'
    @c_file.puts '#define RTE_CORE'
    @c_file.puts ''
    @c_file.puts '/***************************************************************************************************'
    @c_file.puts '**                              Includes                                                          **'
    @c_file.puts '***************************************************************************************************/'
    @c_file.puts '#include "SEL_RTE.h"'
    @c_file.puts ''
    @c_file.puts '#include "Rte_Type.h"'
    @c_file.puts '#include "SEL_DEM.h"'
    @c_file.puts '#include "STDC_A.h"'
    @c_file.puts ''

    @h_file.puts '#ifndef SEL_RTE_H'
    @h_file.puts '#define SEL_RTE_H'
    @h_file.puts ''
    @h_file.puts '/***************************************************************************************************'
    @h_file.puts '**                              Includes                                                          **'
    @h_file.puts '***************************************************************************************************/'
    @h_file.puts '#include "Compiler.h"'
    @h_file.puts '#include "Std_Types.h"'
    @h_file.puts '#include "Rte_Type.h"'

end

def puts_with_proto( text )

  @c_file.puts text

  @h_file.print 'extern '

  if text =~ /(.*)\s*=.*;/
    @h_file.puts "#{$1};"
  elsif text =~ /;\s*$/
    @h_file.puts text
  else
    @h_file.print text.chomp
    @h_file.puts ';'
  end

end

def output_tx_code( line )

  params_text = ''

  if line =~ RTE_FUNC_SEND_QUEUE_OUTPUT
    params_text = 'data'
  elsif line =~ RTE_CCM_HDL
    params_text = 'CcMsgId, CcMsgState, CcMsgPrio, CcMsgResendPeriod, CcMsgBlinkingFrequency, CcMsgFunctionId'
  end

  @h_file.puts     '#if( UNIT_TEST == 1 )'
  @h_file.puts     "  extern void UT_#{$1}_callback(#{$2};"
  @h_file.puts     '#endif'
  @h_file.puts     ''

  puts_with_proto line
  @c_file.puts    '{'
  @c_file.puts    '  #if( CAPL_DLL_BUILD == 1 )'
  @c_file.puts    "    DLL_tx_#{$1}( #{params_text} );"
  @c_file.puts    '  #elif( UNIT_TEST == 1 )'
  @c_file.puts    "    UT_#{$1}_callback( #{params_text} );"
  @c_file.puts    '  #endif'
  @c_file.puts    ''
  @c_file.puts    '    return RTE_E_OK;'
  @c_file.puts    '}'

end

def output_dtc_map()

  enum_count = 0

  dem_file = File.new(@dem_dest_path, 'w')

  output_copyright( dem_file )
  dem_file.puts '#ifndef SEL_DEM_MAP_H'
  dem_file.puts '#define SEL_DEM_MAP_H'
  dem_file.puts ''
  dem_file.puts 'typedef enum'
  dem_file.puts '{'
  @rte_dtc_mapping_hash.each_key do | key |
    dem_file.printf( "    %-40s // %d\n", "DTC_#{key},", enum_count )
    enum_count = enum_count + 1
  end
  dem_file.puts ''
  dem_file.puts '    DTC_NOT_DEFINED,'
  dem_file.puts '    DTC_TOTAL_NUMBER_OF = DTC_NOT_DEFINED'
  dem_file.puts ''
  dem_file.puts '} DTC_et;'
  dem_file.puts ''
  dem_file.puts ''
  dem_file.puts '  #define DTC_MAP_CASES( rtn ) \\'
  @rte_dtc_mapping_hash.each do | key, value |
    dem_file.puts "    case #{value}: rtn = DTC_#{key}; break; \\"
  end
  dem_file.puts "    default:  rtn = DTC_NOT_DEFINED; break;"
  dem_file.puts ''
  dem_file.puts '#endif'

end


def output_rte_sel_functions()

  @c_file.puts ''
  @c_file.puts ''
  @c_file.puts '////////////////////////////////////////////////////////////////'
  @c_file.puts '///////////////////// SEL SPECIFIC STUFF ///////////////////////'
  @c_file.puts '////////////////////////////////////////////////////////////////'
  @c_file.puts ''
  @c_file.puts 'FUNC(void, RTE_CODE) Rte_AVL_QUAN_EES_WHL_pdu_update ( unsigned long * data )'
  @c_file.puts '{'
  @c_file.puts '    IstAnzhlGebrflankRadVLType data_VL;'
  @c_file.puts '    IstAnzhlGebrflankRadVRType data_VR;'
  @c_file.puts '    IstAnzhlGebrflankRadHRType data_HR;'
  @c_file.puts '    IstAnzhlGebrflankRadHLType data_HL;'
  @c_file.puts '    ZahlrSyncIstAnzhlGebrflankRadType timestamp;'
  @c_file.puts ''
  @c_file.puts '    timestamp = ( ZahlrSyncIstAnzhlGebrflankRadType ) data[0];'
  @c_file.puts ''
  @c_file.puts '    data_VL.IstAnzhlGebrflankRadVL = ( IstAnzhlGebrflankRadValueType ) data[1];'
  @c_file.puts '    data_VL.QuIstAnzhlGebrflankRadVL = 0;'
  @c_file.puts ''
  @c_file.puts '    data_VR.IstAnzhlGebrflankRadVR = ( IstAnzhlGebrflankRadValueType ) data[2];'
  @c_file.puts '    data_VR.QuIstAnzhlGebrflankRadVR = 0;'
  @c_file.puts ''
  @c_file.puts '    data_HR.IstAnzhlGebrflankRadHR =  ( IstAnzhlGebrflankRadValueType )data[3];'
  @c_file.puts '    data_HR.QuIstAnzhlGebrflankRadHR = 0;'
  @c_file.puts ''
  @c_file.puts '    data_HL.IstAnzhlGebrflankRadHL =  ( IstAnzhlGebrflankRadValueType )data[4];'
  @c_file.puts '    data_HL.QuIstAnzhlGebrflankRadHL = 0;'
  @c_file.puts ''
  @c_file.puts "    Rte_DataHandleBuffer_#{@rte_dsc_edge_data_buffers["timestamp"]}.value = timestamp;"
  @c_file.puts "    Rte_DataHandleBuffer_#{@rte_dsc_edge_data_buffers["VL"]}.value = data_VL;"
  @c_file.puts "    Rte_DataHandleBuffer_#{@rte_dsc_edge_data_buffers["VR"]}.value = data_VR;"
  @c_file.puts "    Rte_DataHandleBuffer_#{@rte_dsc_edge_data_buffers["HL"]}.value = data_HL;"
  @c_file.puts "    Rte_DataHandleBuffer_#{@rte_dsc_edge_data_buffers["HR"]}.value = data_HR;"
  @c_file.puts ''
  @c_file.puts '    return;'
  @c_file.puts '}'
  @c_file.puts ''
  @c_file.puts ''
  @c_file.puts ''
  @c_file.puts 'FUNC(void, RTE_CODE) Rte_pre_init( void )'
  @c_file.puts '{'
  @c_file.puts '    RdciComHdlA_PreInit();'
  @c_file.puts '    RdciWarn_PreInitA();'
  @c_file.puts ''
  @c_file.puts '    RdciComHdlQm_PreInit();'
  @c_file.puts '    RdciAnz_PreInitQm();'
  @c_file.puts '    RdciDat_PreInitQm();'
  @c_file.puts '}'
  @c_file.puts ''
  @c_file.puts ''
  @c_file.puts 'FUNC(void, RTE_CODE) Rte_init( void )'
  @c_file.puts '{'
  @c_file.puts '    RdciWarn_InitA();'
  @c_file.puts '    RdciAnz_InitQm();'
  @c_file.puts '    RdciDat_InitQm();'
  @c_file.puts '    RdciDat_InitServices();'
  @c_file.puts '    RdciComHdlQm_InitServices();'
  @c_file.puts '}'
  @c_file.puts ''
  @c_file.puts 'FUNC(void, RTE_CODE) Rte_20ms_tick( void )'
  @c_file.puts '{'
  @c_file.puts '    static int tick_cout_20ms_s = 0;'
  @c_file.puts ''
  @c_file.puts '    SwcRdciComHdlA_20ms();'
  @c_file.puts '    RdciWarn_CyclicA();'
  @c_file.puts ''
  @c_file.puts '    SwcRdciComHdlQm_20ms();'
  @c_file.puts '    RdciDat_CyclicQm();'
  @c_file.puts '    RdciAnz_CyclicQm();'
  @c_file.puts ''
  @c_file.puts '    if( tick_cout_20ms_s > 0 )'
  @c_file.puts '    {'
  @c_file.puts '        tick_cout_20ms_s--;'
  @c_file.puts '    }'
  @c_file.puts '    else'
  @c_file.puts '    {'
  @c_file.puts '        /* 100 ms tick */'
  @c_file.puts '        tick_cout_20ms_s = 4;'
  @c_file.puts '        RdciDat_Services();'
  @c_file.puts '        RdciComHdlQm_Services();'
  @c_file.puts '    }'
  @c_file.puts '}'
  @c_file.puts ''
  @c_file.puts 'FUNC(void, RTE_CODE) Rte_pre_fini( void )'
  @c_file.puts '{'
  @c_file.puts ''
  @c_file.puts '}'
  @c_file.puts ''
  @c_file.puts ''
  @c_file.puts 'FUNC(void, RTE_CODE) Rte_fini( void )'
  @c_file.puts '{'
  @c_file.puts '    RdciDat_FiniQm();'
  @c_file.puts '    RdciAnz_FiniQm();'
  @c_file.puts '    RdciWarn_FiniA();'
  @c_file.puts '    RdciComHdlA_Fini();'
  @c_file.puts '    RdciDat_FiniServices();'
  @c_file.puts '    RdciComHdlQm_FiniServices();'
  @c_file.puts '}'
  @c_file.puts ''
  @c_file.puts ''


  @h_file.puts ''
  @h_file.puts ''
  @h_file.puts ''
  @h_file.puts 'FUNC(void, RTE_CODE) Rte_AVL_QUAN_EES_WHL_pdu_update ( unsigned long * data );'
  @h_file.puts 'FUNC(void, RTE_CODE) Rte_pre_init( void );'
  @h_file.puts 'FUNC(void, RTE_CODE) Rte_init( void );'
  @h_file.puts 'FUNC(void, RTE_CODE) Rte_20ms_tick( void );'
  @h_file.puts 'FUNC(void, RTE_CODE) Rte_pre_fini( void );'
  @h_file.puts 'FUNC(void, RTE_CODE) Rte_fini( void );'
  @h_file.puts ''
  @h_file.puts '#endif'
  @h_file.puts ''
end





# Command Line Support ###############################
if ($0 == __FILE__)

  if ARGF.argv.size != 3
    puts "Usage: extract_rtc.rb <source_path> <dest_path> <dem_dest_path>"
    exit
  end

  @src_path      = ARGF.argv[0]
  @dest_path     = ARGF.argv[1]
  @dem_dest_path = ARGF.argv[2]

  @src_path      = File.expand_path(@src_path)
  @dest_path     = File.expand_path(@dest_path)
  @dem_dest_path = "#{File.expand_path(@dem_dest_path)}/SEL_DEM_map.h"


  @rte_dsc_receive_buffers   = Hash.new
  @rte_dsc_edge_data_buffers = Hash.new
  @rte_dtc_mapping_hash      = Hash.new

  rte_buffer_list           = []
  rte_receive_queue_list    = []
  in_rte_instance_block     = false
  in_rte_send_queue         = false
  in_rte_receive_queue      = false
  in_rte_dsc_edge_write     = false
  in_rte_dem_set_event      = false
  lastFuncTypeText          = ''
  lastDemText               = ''


  if File.exists?(@src_path)

    if not Dir.exists?(File.dirname(@dest_path))
       FileUtils.mkdir_p(File.dirname(@dest_path))
    end

    if not Dir.exists?(File.dirname(@dem_dest_path))
       FileUtils.mkdir_p(File.dirname(@dem_dest_path))
    end

    @c_file = File.new(@dest_path, 'w')
    @h_file = File.new(@dest_path.gsub(/\.[cC]/, '.h'), 'w')

    output_headers()


    # 1. Find all the buffer numbers relavent to our software components
    File.open(@src_path, 'r') do |f|
      f.each_line do |line|

        # CONST(Rte_CDS_SwcRdciComHdlA, RTE_CONST) Rte_Instance_SwcRdciComHdlA =
        if line =~ SWC_INSTANCE_START
          in_rte_instance_block = true

        elsif in_rte_instance_block == true

          # Rte_DataHandleBuffer_2041
          if line =~ /Rte_DataHandleBuffer_(\d+)/
            rte_buffer_list << $1.to_i
          elsif line =~ SWC_INSTANCE_END
            in_rte_instance_block = false
          end

        # Rte_Send_SwcRdciComHdlA_RdciComHdlOutputA_RDC_TYR_ID1
        elsif line =~ RTE_FUNC_SEND_QUEUE_OUTPUT
          # Do nothing in this case as we don't need to gather the queue variable information

        # Rte_Send_SwcRdciComHdlA_RdciEinleseinhRdcSensEinlesRdcSens_LfrntId
        elsif line =~ RTE_FUNC_SEND_QUEUE_START
          in_rte_send_queue = true

          if line =~ /P2CONST\s*\(\s*([0-9a-zA-Z_ ]+)\s*,/
            lastFuncTypeText = $1
          elsif line =~ /\(\s*(\w+)\s+data\s*\)/
            lastFuncTypeText = $1
          else
            puts "ERROR - couldn't find lastFuncTypeText in :#{line}!"
            exit 0
          end

        elsif in_rte_send_queue == true
          if line =~ /Rte_ReceiveQueue_(\d+).*%\s*(\d+)/
            if rte_receive_queue_list.find_index( $1.to_i ).nil?
              rte_receive_queue_list << $1.to_i
              length = ( $2.to_i )

              @h_file.puts ''
              @h_file.puts 'typedef struct'
              @h_file.puts '{'
              @h_file.printf( "    %-40s queue[#{length}];\n", lastFuncTypeText)
              @h_file.printf( "    %-40s head;\n", 'unsigned char')
              @h_file.printf( "    %-40s tail;\n", 'unsigned char')
              @h_file.printf( "    %-40s overflow;\n", 'unsigned char')
              @h_file.puts "} Rte_ReceiveQueue_#{$1}_Type;"
              @h_file.puts ''
            end

            in_rte_send_queue = false
          end

        # Rte_Receive_SwcRdciWarn_RdciWarnExtEinleseinhRdcSensEinlesRdcSens_RdcDat
        elsif line =~ RTE_FUNC_RECEIVE_QUEUE_START
          in_rte_receive_queue = true

          if line =~ /P2VAR\s*\(\s*([0-9a-zA-Z_ ]+)\s*,/
            lastFuncTypeText = $1
          elsif line =~ /\(\s*(\w+)\s+data\s*\)/
            lastFuncTypeText = $1
          else
            puts "ERROR - couldn't find lastFuncTypeText in :#{line}!"
            exit 0
          end

        elsif in_rte_receive_queue == true
          if line =~ /Rte_ReceiveQueue_(\d+).*%\s*(\d+)/
            if rte_receive_queue_list.find_index( $1.to_i ).nil?
              rte_receive_queue_list << $1.to_i
              length = ( $2.to_i )

              @h_file.puts ''
              @h_file.puts 'typedef struct'
              @h_file.puts '{'
              @h_file.printf( "    %-40s queue[#{length}];\n", lastFuncTypeText)
              @h_file.printf( "    %-40s head;\n", 'unsigned char')
              @h_file.printf( "    %-40s tail;\n", 'unsigned char')
              @h_file.printf( "    %-40s overflow;\n", 'unsigned char')
              @h_file.puts "} Rte_ReceiveQueue_#{$1}_Type;"
              @h_file.puts ''
            end

            in_rte_receive_queue = false
          end

        elsif line =~ RTE_FUNC_WRITE_ABS_EDGE_START
          in_rte_dsc_edge_write = true

        elsif in_rte_dsc_edge_write == true
          if line =~ /Rte_ReceiveBuffer_(\d+).*IstAnzhlGebrflankRad([VH][RL])Type/
            @rte_dsc_receive_buffers[$2] = $1
            in_rte_dsc_edge_write = false
          elsif line =~ /Rte_ReceiveBuffer_(\d+)/
            @rte_dsc_receive_buffers["timestamp"] = $1
            in_rte_dsc_edge_write = false
          end

        elsif line =~ RTE_FUNC_DEM_CALL_START
          in_rte_dem_set_event = true
          lastDemText = $1

        elsif in_rte_dem_set_event == true
          # Rte_Status = Dem_SetEventStatus(505U, EventStatus);
          if line =~ /Dem_SetEventStatus.*\(\s*(\d+)/
            @rte_dtc_mapping_hash[lastDemText] = $1
            in_rte_dem_set_event = false
          end

        elsif line =~ RTE_INCLUDE
          @c_file.puts line

        elsif line =~ /=\[macros\]=/ # Just for nice tidy file!
          @c_file.puts ''
          @c_file.puts ''
          @c_file.puts '/***************************************************************************************************'
          @c_file.puts '**                              Defines                                                           **'
          @c_file.puts '***************************************************************************************************/'
          @c_file.puts '#define RTE_PARTITION_ACTIVE ( 1 )'
          @c_file.puts '#define TS_MemCpy            STDC_A_memcpy'
          puts_to_both ''
          puts_to_both ''
          puts_to_both '/***************************************************************************************************'
          puts_to_both '**                              The Rest of the file :=)                                          **'
          puts_to_both '***************************************************************************************************/'

        end
      end
    end





    # 2. Extract the actual code information
    in_function_block = false

    @c_file.puts ''
    @c_file.puts '// Keep the copied RTE code happy!'
    @c_file.puts 'STATIC VAR(uint8, RTE_VAR) Rte_State = RTE_PARTITION_ACTIVE;'
    @c_file.puts ''
    @c_file.puts ''

    File.open(@src_path, 'r') do |f|
      f.each_line do |line|

        #VAR(Rte_DE_IstDruckReifnType, RTE_VAR) Rte_DataHandleBuffer_2007;
        if line =~ /^VAR.*Rte_DataHandleBuffer_(\d+)/
          idx = rte_buffer_list.index($1.to_i)

          if not idx.nil?
            rte_buffer_list.delete_at(idx)
            puts_with_proto line
          end

        #STATIC VAR(Rte_ReceiveQueue_120_Type, RTE_VAR) Rte_ReceiveQueue_120;
        elsif line =~ /^STATIC VAR.*Rte_ReceiveQueue_(\d+)/
          idx = rte_receive_queue_list.index($1.to_i)

          if not idx.nil?
            rte_receive_queue_list.delete_at(idx)
            @c_file.puts line
          end

        # CONSTP2CONST(Rte_CDS_SwcRdciComHdlQm, RTE_CONST, RTE_CONST) Rte_Inst_SwcRdciComHdlQm = &Rte_Instance_SwcRdciComHdlQm;
        elsif line =~ /^CONSTP2CONST.+Rte_Instance_SwcRdci\w+;$/
          puts_to_both ''
          puts_with_proto line

        # CONST(Rte_CDS_SwcRdciComHdlA, RTE_CONST) Rte_Instance_SwcRdciComHdlA =
        elsif line =~ SWC_INSTANCE_START
          in_rte_instance_block = true
          @c_file.puts ''
          @c_file.puts line
        elsif in_rte_instance_block == true
          @c_file.puts line

          if line =~ SWC_INSTANCE_END
            in_rte_instance_block = false
            @c_file.puts ''
          end

        # Rte_Send_SwcRdciComHdlA_RdciComHdlOutputA_RDC_TYR_ID1 - special - we don't bother with END
        # Rte_Call_SwcRdciDat_IntCcHdl_SetResetCcMsg
        elsif line =~ RTE_FUNC_SEND_QUEUE_OUTPUT or line =~ RTE_CCM_HDL
          puts_to_both     ''
          output_tx_code( line )

        elsif line =~ RTE_FUNC_NVM_WRITE_BLOCK
          puts_to_both ''
          puts_with_proto line
          @c_file.puts    '{'
          @c_file.puts    '    // Temporarily return the status directly in this call.'
          @c_file.puts    '    // Maybe not the best idea as the calling function needs to expect and understand this simplification fudge!'
          @c_file.puts    "    #{$1}_NvMNotifyJobFinished_JobFinished (0x07, 0);"
          @c_file.puts    '    return RTE_E_OK;'
          @c_file.puts    '}'

        elsif line =~ RTE_FUNC_NVM_SET_RAM_STATUS
          puts_to_both    ''
          puts_with_proto line
          @c_file.puts    '{'
          @c_file.puts    '    // TODO: If needed  NvM_Rte_SetRamBlockStatus(xxU, BlockChanged);'
          @c_file.puts    '    return RTE_E_OK;'
          @c_file.puts    '}'


        # Rte_Send_SwcRdciComHdlA_RdciEinleseinhRdcSensEinlesRdcSens_LfrntId
        # Rte_Receive_SwcRdciWarn_RdciWarnExtEinleseinhRdcSensEinlesRdcSens_RdcDat
        # FUNC(CRdciWarnDelay, RTE_CODE) Rte_CData_TrwEspArIf_CRdciWarnDelay
        elsif line =~ RTE_FUNC_SEND_QUEUE_START    or
              line =~ RTE_FUNC_RECEIVE_QUEUE_START or
              line =~ RTE_FUNC_CALPRM_CALL_START   or
              line =~ RTE_FUNC_DEM_CALL_START
          in_function_block = true
          puts_to_both    ''
          puts_with_proto line

        elsif in_function_block == true
          if line !~ /Rte_Int(Unlock|Lock)/ and line !~ /Hook/ and line !~ /Runnable/
            @c_file.puts line
          end

          if line =~ FUNC_RETURN_POINT
            in_function_block = false
            @c_file.puts '}'
            @c_file.puts ''
          end

        elsif line =~ RTE_DSC_EDGE_BUFFER_MAP

          @rte_dsc_receive_buffers.each do | key, value |
             if value.to_i == $2.to_i
                if @rte_dsc_edge_data_buffers.has_key?(key)
                  # Assumption here is that the highest valued buffer is the non pre_init buffer!
                  if $1.to_i < @rte_dsc_edge_data_buffers[key]
                    @rte_dsc_edge_data_buffers[key] = $1.to_i
                  end
                else
                  @rte_dsc_edge_data_buffers[key] = $1.to_i
                end
             end

          end

        elsif line =~ RTE_PIM or line =~ RTE_VAR_CALPRM
          puts_with_proto line

        # Uncomment these to see if we have caovered all the interesting stuff!
        #elsif line =~ /Rdci/ and line !~ /Runnable/ and line !~ /Hook/ and line !~ /\/\*.+\*\// and line !~ RTE_FUNC_WRITE_ABS_EDGE_START
        #  puts line
        end
      end

      output_rte_sel_functions()
      output_dtc_map()

    end

  else
    puts "Error: Source path '#{@src_path}' does not exist."
  end

end

