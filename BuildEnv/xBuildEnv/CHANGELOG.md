# Change Log
All notable changes to this project will be documented in this file, format is based on http://keepachangelog.com/

Please view this file on the master branch, on stable branches it's out of date.

## [v2.1.3.RC] - 2016-09-16

### Added 
 - sVersion, autoversion feature now allows directory to be passed via commandline, autoversion directory priority is commandline, 'src' then current directories.  See #29
 - svn-tools-check tool that warns when SVN_Tools is out of date. See #14
 - ad-tool for easy interrogation of Active Directory
 - BENV_USERNAME environment variable that maps to old SEL style login usernames for use in generated code. see #30
 - sgit-subdir debug option
 - smart-license-manager that "Manage retries in the event a license is busy". see #25

### Fixed
 - Optimised sgit-subdir search algorithm to speed up check and list commands.  See #28
 - qac-fixpaths optimisations to include acf files and now automagically tries to base files from project root folder (PTLD). See #22
 - sVersion inject and cross check features removed since problematic and now redundant with introduction of autoversion feature.  See #31
 - sgit-subdir update commits any non-versioned files if present within subfolders.  See #36
 - Missing CHANGELOG.md when using sgit-subdir add \ update commands.  See #39
 - sgit-subdir list does not recognise subdirs when parent directory is renamed.  See #32
 - sgit-subdir mistakenly commits hidden files (e.g. .gitkeep) if present within subdir. See #34
 - cc-wrap output not strictly conforming to gcc style breaking Jenkins parser. see #38

### Changed
 - sgit-flow release finish merge request templates to help avoid incorrect selection of "remove source" option. see #40
 - sgit-flow feature merge request templates updated to provider fuller template.  Goes some way towards addressing #4


## [v2.1.2.RC] - 2016-06-22

### Added 
 - sVersion, autoversion feature to generate an version header file based on project.yml, current Git branch & build id.  See #2

### Fixed
 - sVersion now excludes build id from tag name.
 - sgit-subdir, check now reports line number of 1 & correct relative path to working directory, check and list commands now optimised for speed.  See #26
 - sVersion, now only runs a quick check by default, and only a full check for a tag request. See #2
 - sgit-flow, incorrectly creating merge request ID and links when the SITS link in the name was beyond 999.

## [v2.1.1] - 2016-05-25

### Added 
- Python27 paths to Core BuildEnv to avoid need for every project to setup within project specific setup_env.bat (Required for gcovr coverage report)
- support for detecting if subdir is up to date for both list and check commands

### Fixed 
- sgit-flow failing to identify to match local repo with remote repo
- sgit-flow @all spamming issue, by removing @all from merge request templates. See #19
- sgit-subdir incorrect detection of a root subdir, if directory had an 'x' in the path.  e.g. ttpm-corax3-global-a
- "ceedling-create-env --help" now works. See#16

## [v2.1] - 2016-04-19
### Added
- Initial release of ceedling-create-env tool
- Initial release of qac-fixpaths tool, to correct paths within QAC's project.xml when project is checked out to a different drive or directory
- Old TiddlyWiki Readme.htm ported to README.md
- Initial release of sgit-subdir tool
- Initial release of sgit-flow tool.  See http://sanpgit01.schrader.local/tools/buildenv/wikis/sgit-flow for usage

### Fixed 
- Deeply nested location (long path) causing SelBuildEnv.exe to fail to open.  See #7
- Corrected ceedling-gen-report which was outputting a yml results file to a hard coded app/build directory.  See #12
- Renamed genCeedlingReport to ceedling-gen-report
- Corrected sVersion tool to support the official SEL version number format of X.Y.Z.ReleaseType with dot before ReleaseType
- Changed genCModule tool to specify enums before data structures
