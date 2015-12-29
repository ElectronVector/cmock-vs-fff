#include "unity.h"
#include "event_processor.h"
#include "mock_display.h"
#include <string.h>

void setUp(void)
{
}

void tearDown(void)
{
}

/*
    Test that a single function was called.
*/
void
test_whenTheDeviceIsReset_thenTheStatusLedIsTurnedOff(void)
{
	// Then
	display_turnOffStatusLed_Expect();

	// When
	event_deviceReset();
}

/*
    Test that a single function is NOT called.
*/
void
test_whenThePowerReadingIsLessThan5_thenTheStatusLedIsNotTurnedOn(void)
{
	// Then

	// When
	event_powerReadingUpdate(4);
}

/*
	Test that a single function was NOT called, while other functions may be
	called.
*/
void
test_whenTheDeviceIsReset_thenTheStatusLedIsNotTurnedOn(void)
{
	// Then
	// NOTE: We have to ignore a function we know could be called.
	display_turnOffStatusLed_Ignore();

	// When
	event_deviceReset();
}

/*
    Test that no functions were called.
*/

/*
	Test that a single function was called with the correct arugment.
*/
void
test_whenTheVolumeKnobIsMaxed_thenVolumeDisplayIsSetTo11(void)
{
	// Then
	display_setVolume_Expect(11);

	// When
	event_volumeKnobMaxed();
}

/*
	Test a that a sequence of calls occurred.
*/

void
test_whenTheModeSelectButtonIsPressed_thenTheDisplayModeIsCycled(void)
{
	// Then
	display_setModeToMinimum_Expect();
	display_setModeToMaximum_Expect();
	display_setModeToAverage_Expect();

	// When
	event_modeSelectButtonPressed();
	event_modeSelectButtonPressed();
	event_modeSelectButtonPressed();
}

/*
	Mock a return value from a function.
*/
void
test_givenTheDisplayHasAnError_whenTheDeviceIsPoweredOn_thenTheDisplayIsPoweredDown(void)
{
	// Given
	display_isError_ExpectAndReturn(true);

	// Then
	display_powerDown_Expect();

	// When
	event_devicePoweredOn();
}

/*
	Mock a sequence of calls with return values.
*/

/*
	Mocking a function with a value returned by reference.

	Note that this requires enabling the "return_thru_ptr" and "ignore_arg"
	plugins for CMock. This is done in project.yml by adding to the list of
	CMock plugins:

	:cmock:
	  :plugins:
	    - :ignore
	    - :callback
	    - :return_thru_ptr 		<---- Enable this plugin.
		- :ignore_arg			<---- And this one.
*/
void
test_givenTheUserHasTypedSleep_whenItIsTimeToCheckTheKeyboard_theDisplayIsPoweredDown(void)
{
	// Given
	char entry[] = "sleep";
	display_getKeyboardEntry_Expect(0,0);
	display_getKeyboardEntry_ReturnArrayThruPtr_entry(entry, strlen(entry));
	display_getKeyboardEntry_IgnoreArg_entry();
	display_getKeyboardEntry_IgnoreArg_length();

	// Then
	display_powerDown_Expect();

	// When
	event_keyboardCheckTimerExpired();
}

/*
	Mock a function with a function pointer parameter.

*/
void
test_givenNewDataIsAvailable_whenTheDisplayHasUpdated_thenTheEventIsComplete(void)
{
	// A mock function for capturing the callback handler function pointer.
	void(*registeredCallback)(void) = 0;
	void mock_display_updateData(int data, void(*callback)(void), int numCalls)
	{
		//Save the callback function.
		registeredCallback = callback;
	}

	// Given
	display_updateData_StubWithCallback(mock_display_updateData);
	event_newDataAvailable(10);

	// When
	if (registeredCallback != 0)
	{
		registeredCallback();
	}

	// Then
	TEST_ASSERT_EQUAL(true, eventProcessor_isLastEventComplete());
}
