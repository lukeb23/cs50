-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Get the full report of the crime scene on the day of the robbery --

SELECT description FROM crime_scene_reports WHERE
year = 2021 AND
street = "Humphrey Street" AND
month = 7 AND
day = 28;

-- Check for the interviews of the people at the bakery and their names --

SELECT transcript FROM interviews WHERE transcript LIKE '%bakery%';

-- Consolidate all of the three pieces of info given at the scene prior into 1 query search of names --
SELECT distinct(name), passport_number
FROM people
JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
JOIN phone_calls ON people.phone_number = phone_calls.caller
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
JOIN bank_accounts ON people.id = bank_accounts.person_id
WHERE caller
IN (
        SELECT caller FROM phone_calls WHERE
        year = 2021 AND
        month = 7 AND
        day = 28 AND
        duration < 60
   )
AND bakery_security_logs.license_plate
IN (
        SELECT license_plate FROM bakery_security_logs
        WHERE year = 2021 AND
        hour = 10 AND
        minute > 15 AND
        minute < 25 AND
        month = 7 AND
        day = 28
    )
AND atm_transactions.account_number
IN (
    SELECT account_number FROM atm_transactions WHERE
    atm_location = "Leggett Street" AND
    year = 2021 AND
    month = 7 AND
    day = 28 AND
    transaction_type = "withdraw"
);

-- Get the airport of fiftyville--
SELECT id FROM airports
WHERE city = "Fiftyville";

-- Find Out Where the Theif Escaped to --
SELECT destination_airport_id, id, hour FROM flights
WHERE origin_airport_id = 8 AND
year = 2021 AND
month = 7 AND
day = 29
ORDER BY hour;

-- Find the name of the city the theives escaped to --
SELECT city FROM airports WHERE
id = 4;

-- Find out which person the theif was by looking at passport numbers of the earliest flight --
SELECT seat FROM passengers WHERE
flight_id = 36 AND
passport_number = "5773159633" OR
passport_number = "3592750733";


-- Find out accomplice via the phone number Bruce had been talking to --
SELECT name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.receiver
WHERE caller IN
(SELECT phone_number FROM  people WHERE
name = "Bruce")
AND year = 2021 AND
month = 7 AND
day = 28 AND
duration < 60;
