#include <stdio.h>
#include "minunit.h"
#include "event_processor.h"

#include <stdbool.h>
#include <string.h>
#include "fff.h"
DEFINE_FFF_GLOBALS;

int tests_run = 0;
int tests_failed = 0;

FAKE_VOID_FUNC(display_turnOffStatusLed);
FAKE_VOID_FUNC(display_turnOnStatusLed);
FAKE_VOID_FUNC(display_setVolume, int);
FAKE_VOID_FUNC(display_setModeToMinimum);
FAKE_VOID_FUNC(display_setModeToMaximum);
FAKE_VOID_FUNC(display_setModeToAverage);
FAKE_VALUE_FUNC(bool, display_isError);
FAKE_VOID_FUNC(display_powerDown);
FAKE_VOID_FUNC(display_getKeyboardEntry, char *, int);

typedef void (*displayCompleteCallback) (void);
FAKE_VOID_FUNC(display_updateData, int, displayCompleteCallback);

void setUp (void)
{
    //Reset the FFF call history between tests.
    FFF_RESET_HISTORY();

    //FFF requires resetting the fake each time it is used.
    RESET_FAKE(display_turnOffStatusLed);
    RESET_FAKE(display_turnOnStatusLed);
    RESET_FAKE(display_setVolume);
    RESET_FAKE(display_setModeToMinimum);
    RESET_FAKE(display_setModeToMaximum);
    RESET_FAKE(display_setModeToAverage);
    RESET_FAKE(display_isError);
    RESET_FAKE(display_powerDown);
    RESET_FAKE(display_getKeyboardEntry);
}

void tearDown (void)
{
}

/*
    Test that a single function was called.
*/
void
test_whenTheDeviceIsReset_thenTheStatusLedIsTurnedOff()
{
    // When
    event_deviceReset();

    // Then
    TEST_ASSERT_EQUAL(1, display_turnOffStatusLed_fake.call_count);
}

/*
    Test that a single function is NOT called.
*/
void
test_whenThePowerReadingIsLessThan5_thenTheStatusLedIsNotTurnedOn(void)
{
	// When
    event_powerReadingUpdate(4);

	// Then
    TEST_ASSERT_EQUAL(0, display_turnOnStatusLed_fake.call_count);
}

/*
    Test that no functions were called.
*/

/*
    Test that a single function was NOT called, while other functions may be
    called.
*/
void
test_whenTheDeviceIsReset_thenTheStatusLedIsNotTurnedOn()
{
    // When
    event_deviceReset();

    // Then
    // NOTE: This is the same as the previous case.
    TEST_ASSERT_EQUAL(0, display_turnOnStatusLed_fake.call_count);
}

/*
	Test that a single function was called with the correct arugment.
*/
void
test_whenTheVolumeKnobIsMaxed_thenVolumeDisplayIsSetTo11(void)
{
	// When
	event_volumeKnobMaxed();

	// Then
	TEST_ASSERT_EQUAL(1, display_setVolume_fake.call_count);
    TEST_ASSERT_EQUAL(11, display_setVolume_fake.arg0_val);
}

/*
	Test a sequence of calls.
*/

void
test_whenTheModeSelectButtonIsPressed_thenTheDisplayModeIsCycled(void)
{
	// When
	event_modeSelectButtonPressed();
	event_modeSelectButtonPressed();
	event_modeSelectButtonPressed();

    // Then
    TEST_ASSERT_EQUAL(fff.call_history[0], (void *)display_setModeToMinimum);
    TEST_ASSERT_EQUAL(fff.call_history[1], (void *)display_setModeToMaximum);
    TEST_ASSERT_EQUAL(fff.call_history[2], (void *)display_setModeToAverage);
}

/*
	Mock a return value from a function.
*/
void
test_givenTheDisplayHasAnError_whenTheDeviceIsPoweredOn_thenTheDisplayIsPoweredDown(void)
{
	// Given
	display_isError_fake.return_val = true;

	// When
	event_devicePoweredOn();

    // Then
    TEST_ASSERT_EQUAL(1, display_powerDown_fake.call_count);
}

/*
	Mock a sequence of calls with return values.
*/

/*
	Mocking a function with a value returned by reference.
*/
void
test_givenTheUserHasTypedSleep_whenItIsTimeToCheckTheKeyboard_theDisplayIsPoweredDown(void)
{
	// Given
    char mockedEntry[] = "sleep";
    void return_mock_value(char * entry, int length)
    {
        if (length > strlen(mockedEntry))
        {
            strncpy(entry, mockedEntry, length);
        }
    }
    display_getKeyboardEntry_fake.custom_fake = return_mock_value;

	// When
	event_keyboardCheckTimerExpired();

    // Then
    TEST_ASSERT_EQUAL(1, display_powerDown_fake.call_count);
}

/*
	Mock a function with a function pointer parameter.

*/
void
test_givenNewDataIsAvailable_whenTheDisplayHasUpdated_thenTheEventIsComplete(void)
{
	// A mock function for capturing the callback handler function pointer.
	void(*registeredCallback)(void) = 0;
	void mock_display_updateData(int data, void(*callback)(void))
	{
		//Save the callback function.
		registeredCallback = callback;
	}
    display_updateData_fake.custom_fake = mock_display_updateData;

	// Given
	event_newDataAvailable(10);

	// When
	if (registeredCallback != 0)
	{
		registeredCallback();
	}

	// Then
	TEST_ASSERT_EQUAL(true, eventProcessor_isLastEventComplete());
}

void all_tests()
{
    RUN_TEST(test_whenTheDeviceIsReset_thenTheStatusLedIsTurnedOff);
    RUN_TEST(test_whenThePowerReadingIsLessThan5_thenTheStatusLedIsNotTurnedOn);
    RUN_TEST(test_whenTheDeviceIsReset_thenTheStatusLedIsNotTurnedOn);
    RUN_TEST(test_whenTheVolumeKnobIsMaxed_thenVolumeDisplayIsSetTo11);
    RUN_TEST(test_whenTheModeSelectButtonIsPressed_thenTheDisplayModeIsCycled);
    RUN_TEST(test_givenTheDisplayHasAnError_whenTheDeviceIsPoweredOn_thenTheDisplayIsPoweredDown);
    RUN_TEST(test_givenTheUserHasTypedSleep_whenItIsTimeToCheckTheKeyboard_theDisplayIsPoweredDown);
    RUN_TEST(test_givenNewDataIsAvailable_whenTheDisplayHasUpdated_thenTheEventIsComplete);
}

int main(int argc, char **argv) {
    all_tests();

    printf("\n");
    printf("--------------------\n");
    printf("OVERALL TEST SUMMARY\n");
    printf("--------------------\n");
    printf("TESTED:  %d\n", tests_run);
    printf("PASSED:  %d\n", tests_run - tests_failed);
    printf("FAILED:  %d\n", tests_failed);

    return 0;
}
