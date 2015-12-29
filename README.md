# CMock vs fff

Comparison of CMock and fff (Fake Function Framework) usage.

Both are used to implement the same tests on an example "event processor"
module.

## Tests

The CMock tests are in test/test_event_processor_cmock.c. Run them with `rake
cmock`.

The fff tests are in src/test_event_processor_fff.c. Run them with `rake fff`.
