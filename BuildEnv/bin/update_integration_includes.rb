#!/usr/bin/env ruby

require 'FileUtils'

SEL_MODULE_FILE  = /\b(([A-Z]+)(_[A-Z]+)*)(\w*)\.([cChH])/

def get_source_files()

  list = Dir["**/*.[cC]"]

  list.delete_if { |x| x =~ /template/i }
  list.delete_if { |x| x !~ SEL_MODULE_FILE }
  list.delete_if { |x| x =~ /_common/ }

  list
end

# Command Line Support ###############################
if ($0 == __FILE__)

  if ARGF.argv.size != 2
    puts "Usage: update_integration_headers.rb <tests_path> <src_path>"
    exit
  end

  tests_dir  = ARGF.argv[0]
  src_dir    = ARGF.argv[1]

  tests_dir  = File.expand_path(tests_dir)
  src_dir    = File.expand_path(src_dir)

  if Dir.exists?(tests_dir) and Dir.exists?(src_dir)

	  # Figure out all the standard SEL naming convention header files
    FileUtils.cd(src_dir)
    src_files = get_source_files()

	  headers = []
	  src_files.each do |src_name|
      if src_name =~ /\.C$/
        basename = File.basename( src_name, ".C" )
      else
        basename = File.basename( src_name, ".c" )
      end
	    headers << "#{basename}.h"
      puts "Creating include for module #{basename}"
	  end

	  headers.uniq!
	  headers.sort!


	  # Now put them into the test files!
    FileUtils.cd(tests_dir)
    test_files = Dir["**/test_*.[cC]"]

    test_files.each do |test_file|

	    contents = File.readlines( test_file )

      found_begin = false
      special_comments_present = false

      contents.each do |line|
        if line =~ /BEGIN_AUTO_INSERTED_HEADERS/
          found_begin = true
        elsif line =~ /END_AUTO_INSERTED_HEADERS/
          if found_begin == true
            special_comments_present = true
            break
          end
        end
      end

      if special_comments_present
        puts "Updating test file #{test_file} with new headers"

        File.open(test_file, 'w') do |f|
          in_headers = false

          contents.each do |line|

            if line =~ /BEGIN_AUTO_INSERTED_HEADERS/
              in_headers = true

              f.puts line
              f.puts '/// The following provide direct access to the RTE functions and variables'
              f.puts '#include "Rte_SwcRdciWarn_Type.h"'
              f.puts '#include "Rte_SwcRdciDat_Type.h"'
              f.puts '#include "Rte_SwcRdciAnz_Type.h"'
              f.puts '#include "Rte_SwcRdciComHdlQm_Type.h"'
              f.puts '#include "Rte_SwcRdciComHdlA_Type.h"'
              f.puts '#include "Rte_SwcRdciErfs_Type.h"'
              f.puts '#define RTE_CORE'
              f.puts '#include "Rte_SwcRdciWarn.h"'
              f.puts '#include "Rte_SwcRdciDat.h"'
              f.puts '#include "Rte_SwcRdciAnz.h"'
              f.puts '#include "Rte_SwcRdciComHdlQm.h"'
              f.puts '#include "Rte_SwcRdciComHdlA.h"'
              f.puts '#include "Rte_SwcRdciErfs.h"'
              f.puts ''
              f.puts ''
              f.puts '/// The following includes cause the corresponding .c file to be built.  These are required for this'
              f.puts '/// integration test module to be compiled into an executable.  If you have a c file that does not have'
              f.puts '/// a corresponding .h file, please place an empty dummy .h file in the support folder.'
              f.puts '// Supporting files'
              f.puts '#include "SEL_RTE.h"'
              f.puts '#include "SEL_DEM.h"'
              f.puts '#include "SEL_NVM.h"'
              f.puts '#include "CRC.h"'
              f.puts '#include "init_helper.h"'
              f.puts '#include "callbacks.h"'
              f.puts '#include "io_helper.h"'
              f.puts '#include "locate_helper.h"'
              f.puts '#include "placard_helper.h"'
              f.puts '#include "sensor_helper.h"'
              f.puts '#include "sensor_conti_helper.h"'
              f.puts '#include "vda_sensor_helper.h"'
              f.puts '#include "test_def.h"'
              f.puts '#include "tick_helper.h"'
              f.puts '#include "compare_helper.h"'
              f.puts '#include "fbd_helper.h"'
              f.puts '#include "vlog_helper.h"'

              f.puts ''
              f.puts '// Core library files'

              headers.each do |header|
                f.puts "#include \"#{header}\""
              end

            elsif line =~ /END_AUTO_INSERTED_HEADERS/
              in_headers = false
              f.puts line
            elsif not in_headers
              f.puts line
            end

          end #contents each

        end # File output do

      else
        puts "Skipping test file #{test_file} because I couldn't find BEGIN_AUTO_INSERTED_HEADERS and END_AUTO_INSERTED_HEADERS comments"
      end

    end # test_files each

  else
    puts "Error: Source path '#{src_dir}' or Tests path '#{tests_dir}' does not exist or is not a directory."
  end

end

