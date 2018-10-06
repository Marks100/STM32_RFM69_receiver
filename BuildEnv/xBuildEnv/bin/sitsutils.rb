#!/usr/bin/env ruby

# Add to require search path
$: << "/SVN_source/Tools/RubyScripts"

require 'docopt'
require "json"
require "net/http"
require 'pp'
require "uri"

# create new string methods
class String
  # normalise all line endings to \n
  def normalize_line_endings
    gsub /\r\n?/, "\n"
  end

  # remove non breaking spaces.  Seems to be Jama adding these
  def rm_nbs
    gsub("\u00A0", " ")
  end

  # indent all lines by 4 spaces
  def indent
    gsub /\n/, "\n    "
  end
end

def rest_req( url )
    uri = URI.parse(url)

    http = Net::HTTP.new(uri.host, uri.port)
    request = Net::HTTP::Get.new(uri.request_uri)
    request.add_field 'Content-Type', 'application/json'
    request.add_field 'Accept', 'application/json'

    puts "Need SITS credentials to login, no peeking..."
    puts "Enter SITS username"
    user_name = $stdin.gets.chomp
    puts "Enter password?"
    user_password = $stdin.gets.chomp

    request.basic_auth("#{user_name}", "#{user_password}")
    response = http.request(request)

    if response.code == "200"
        result = JSON.parse(response.body)
    elsif response.code == "401"
        raise "SITS Login failed"
        result = nil
    else
        raise "Unknown http response"
        result = nil
    end

    result
end

class SitsIssue

  SITS_URL = "http://sits.schrader.co.uk:8080"

  # the following custom fields are dependent on the Jira installation, they will likely change for a different server
  ACCEPTANCE_CRIT = "customfield_12911"
  EPIC_ID = "customfield_11711"
  JAMA_URL = "customfield_10912"
  attr_reader :project_key, :project_name, :key, :summary, :description, :id, :acceptance_criteria, :epic, :components, :fixVersions, :jama_url
  def initialize(id)
    @id = id

    # read issue from SITS
    url = "#{SITS_URL}/rest/api/2/issue/#{@id}"
    result = rest_req( url )

    # Gherkin gem does not like anything weird charachters so remove anything non ascii from any used strings
    encoding_options = {
        :invalid           => :replace,  # Replace invalid byte sequences
        :undef             => :replace,  # Replace anything not defined in ASCII
        :replace           => '',        # Use a blank for those replacements
        :universal_newline => true       # Always break lines with \n
      }

    # store off interesting info
    @project_key = result["fields"]["project"]["key"]
    @project_name = result["fields"]["project"]["name"]
    @key = result["key"]
    @summary = result["fields"]["summary"].encode(Encoding.find('ASCII'), encoding_options)
    @description = result["fields"]["description"].encode(Encoding.find('ASCII'), encoding_options)
    @acceptance_criteria = result["fields"][ACCEPTANCE_CRIT].normalize_line_endings.rm_nbs.indent.encode(Encoding.find('ASCII'), encoding_options)
    @epic = result["fields"][EPIC_ID]
    @components = []
    @components << result["fields"]["components"]
    @fixVersions = []
    @fixVersions << result["fields"]["fixVersions"]
    @jama_url = result["fields"][JAMA_URL]

  end

  def to_s
    string = "Id: " + @id
    string = string + "\nproject_key: " + @project_key
    string = string + "\nproject_name: " + @project_name
    string = string + "\nsummary: " + @summary
    string = string + "\ndescription: " + @description
    string = string + "\nacceptance_criteria: " + @acceptance_criteria
    string = string + "\nepic: " + @epic
    string = string + "\njama_url: " + @jama_url
  end

end

#***************************************************************************************************
#   \brief          Main Program
#***************************************************************************************************
if ($0 == __FILE__)

# Grab just the filename for convenience
NAME = File.basename($0)
PTLD = File.expand_path(ENV['PTLD'])
RX_ABS_PATH = /[A-Za-z]:[\/\\]/

# Version information string
VERSION_NUM = "1.0"
VERSION = <<END_VERSION
########################################################################
#  #{NAME} #{VERSION_NUM}
#  Various SITS utilities
#  Copyright (C) 2015 Sensata Technologies.
########################################################################
END_VERSION

# Build the options!  Notice that #{VERSION} will include a newline at end
DOC_OPT = <<DOCOPT
#{VERSION}
Usage:
  #{NAME} <sits_issue>
  #{NAME} -h | --help
  #{NAME} --version

Options:
  -h --help     Show this screen.
  -v --version  Show version.
DOCOPT

end


if ($0 == __FILE__)

  # Extract options!
  options = {}
  begin
    options = Docopt::docopt(DOC_OPT)
  rescue Docopt::Exit => e
    puts e.message
    exit
  end

  @sits_issue = SitsIssue.new( options["<sits_issue>"] )
  puts @sits_issue.to_s

end