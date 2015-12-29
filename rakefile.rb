PROJECT_CEEDLING_ROOT = "vendor/ceedling"
load "#{PROJECT_CEEDLING_ROOT}/lib/ceedling.rb"

Ceedling.load_project

task :default => %w[ test:all release ]

desc "Run the Cmock-based version of the tests"
task :cmock => %w[ test:all ]

desc "Run the FFF-based version of the tests"
task :fff do
    #Build the test binary.
    sh "gcc src/event_processor.c src/test_event_processor_fff.c -o fff_tests.exe"
    sh "./fff_tests.exe"
end
