-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Check tables
.tables

-- Check how this table was created
.schema crime_scene_reports

----------------------------------------------------------------------------------
-- Check the crime description on July 28, 2024
SELECT description
FROM crime_scene_reports
WHERE year = 2024
AND month = 7
AND day = 28;
-- I was able to know that the crime took place at 10:15pm at Humphrey Street bakery.
-- Three witnesses was interviwed and mentioned the bakery.
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Check how the bakery_security_logs was created
.schema bakery_security_logs

-- Check the bakery security logs on July 28, 2024
SELECT license_plate, hour, minute
FROM bakery_security_logs
WHERE year = 2024
AND month = 7
AND day = 28
AND hour >= 10
AND hour < 11
AND activity = 'exit';

-- +---------------+------+--------+
-- | license_plate | hour | minute |
-- +---------------+------+--------+
-- | 5P2BI95       | 10   | 16     |
-- | 94KL13X       | 10   | 18     |
-- | 6P58WS2       | 10   | 18     |
-- | 4328GD8       | 10   | 19     |
-- | G412CB7       | 10   | 20     |
-- | L93JTIZ       | 10   | 21     |
-- | 322W7JE       | 10   | 23     |
-- | 0NTHK55       | 10   | 23     |
-- | 1106N58       | 10   | 35     |
-- +---------------+------+--------+

-- Now I have all license_plates that exited the place about the hour
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Check how the interview table was created
.schema interviews

-- Check the interview transcripts and the name of the interviewed+person
--on July 28, 2024
SELECT name, transcript
FROM interviews
WHERE year = 2024
AND month = 7
AND day = 28;

-- Raymond: As the thief was leaving the bakery, they called someone who talked to them for less
-- than a minute. In the call, I heard the thief say that they were planning to take the earliest
-- flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to
-- purchase the flight ticket.
-- Lily: Our neighboring courthouse has a very annoying rooster that crows loudly at
-- 6am every day. My sons Robert and Patrick took the rooster to a city far, far away, so it may
-- never bother us again. My sons have successfully arrived in Paris.

-- I should look the atm_transactios table
-- The thief plans to get a flight and asked to the person to buy the flight ticket
--Also the thief called someone who talked to them for less than a minute.
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Check how the atm_transactions table was created
.schema atm_transactions

-- Check the atm transaction amount and account number.
--The atm should be at the Legget Street and the transaction should be
--on July 28, 2024 and transaction_type should be withdraw
SELECT amount, account_number
FROM atm_transactions
WHERE atm_location LIKE '%Leggett Street%'
AND transaction_type = 'withdraw'
AND year = 2024
AND month = 7
AND day = 28;

-- On the day I have eight withdraw transactions
-- +--------+----------------+
-- | amount | account_number |
-- +--------+----------------+
-- | 48     | 28500762       |
-- | 20     | 28296815       |
-- | 60     | 76054385       |
-- | 50     | 49610011       |
-- | 80     | 16153065       |
-- | 20     | 25506511       |
-- | 30     | 81061156       |
-- | 35     | 26013199       |
-- +--------+----------------+
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Check how the bank_accounts table was created
.schema bank_accounts

-- Check the bank_accounts to verify the account_number and get the person_id
-- and the creation_year
SELECT bank_accounts.account_number, bank_accounts.person_id, bank_accounts.creation_year
FROM atm_transactions, bank_accounts
WHERE atm_transactions.atm_location LIKE '%Leggett Street%'
AND atm_transactions.transaction_type = 'withdraw'
AND atm_transactions.year = 2024
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND bank_accounts.account_number = atm_transactions.account_number;

-- +----------------+-----------+---------------+
-- | account_number | person_id | creation_year |
-- +----------------+-----------+---------------+
-- | 28500762       | 467400    | 2014          |
-- | 28296815       | 395717    | 2014          |
-- | 76054385       | 449774    | 2015          |
-- | 49610011       | 686048    | 2010          |
-- | 16153065       | 458378    | 2012          |
-- | 25506511       | 396669    | 2014          |
-- | 81061156       | 438727    | 2018          |
-- | 26013199       | 514354    | 2012          |
-- +----------------+-----------+---------------+

-- Now that I have all the account numbers associated with their person_id
-- I can look on the people table

----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Check how the people table was created
.schema people

-- Check the name, phone_number, passport_number and license_plate
SELECT people.name, people.phone_number, people.passport_number, people.license_plate
FROM people, bank_accounts, atm_transactions
WHERE atm_transactions.atm_location LIKE '%Leggett Street%'
AND atm_transactions.transaction_type = 'withdraw'
AND atm_transactions.year = 2024
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND bank_accounts.account_number = atm_transactions.account_number
AND people.id = bank_accounts.person_id;

-- +---------+----------------+-----------------+---------------+
-- |  name   |  phone_number  | passport_number | license_plate |
-- +---------+----------------+-----------------+---------------+
-- | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
-- | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
-- | Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       |
-- | Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
-- | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
-- | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
-- | Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
-- | Benista | (338) 555-6650 | 9586786673      | 8X428L0       |
-- +---------+----------------+-----------------+---------------+

-- Check the data relating with the bakery_security_logs
SELECT bakery_security_logs.license_plate, people.name, people.phone_number, people.passport_number
FROM bakery_security_logs, people, bank_accounts, atm_transactions
WHERE bakery_security_logs.year = 2024
AND bakery_security_logs.month = 7
AND bakery_security_logs.day = 28
AND bakery_security_logs.activity = 'exit'
AND bakery_security_logs.hour >= 10
AND bakery_security_logs.hour < 11
AND people.license_plate = bakery_security_logs.license_plate
AND atm_transactions.atm_location LIKE '%Leggett Street%'
AND atm_transactions.transaction_type = 'withdraw'
AND atm_transactions.year = 2024
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND bank_accounts.account_number = atm_transactions.account_number
AND people.id = bank_accounts.person_id;

-- +---------------+--------+----------------+-----------------+
-- | license_plate |  name  |  phone_number  | passport_number |
-- +---------------+--------+----------------+-----------------+
-- | 94KL13X       | Bruce  | (367) 555-5533 | 5773159633      |
-- | 322W7JE       | Diana  | (770) 555-1861 | 3592750733      |
-- | L93JTIZ       | Iman   | (829) 555-5269 | 7049073643      |
-- | 4328GD8       | Luca   | (389) 555-5198 | 8496433585      |
-- | 1106N58       | Taylor | (286) 555-6063 | 1988161715      |
-- +---------------+--------+----------------+-----------------+

--

----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Check how the phone_calls table was created
.schema phone_calls

-- Check caller, receiver and duration of the call, with the duration less than
-- a minute, on July 28, 2024
SELECT caller, receiver, duration
FROM phone_calls
WHERE year = 2024
AND month = 7
AND day = 28
AND duration < 60;

SELECT bakery_security_logs.license_plate, people.name,
people.phone_number, people.passport_number, phone_calls.receiver
FROM bakery_security_logs, people, bank_accounts, atm_transactions, phone_calls
WHERE bakery_security_logs.year = 2024
AND bakery_security_logs.month = 7
AND bakery_security_logs.day = 28
AND bakery_security_logs.activity = 'exit'
AND bakery_security_logs.hour >= 10
AND bakery_security_logs.hour < 11
AND people.license_plate = bakery_security_logs.license_plate
AND atm_transactions.atm_location LIKE '%Leggett Street%'
AND atm_transactions.transaction_type = 'withdraw'
AND atm_transactions.year = 2024
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND bank_accounts.account_number = atm_transactions.account_number
AND people.id = bank_accounts.person_id
AND (phone_calls.caller = people.phone_number OR phone_calls.receiver = people.phone_number)
AND phone_calls.duration < 60;

-- +---------------+--------+----------------+-----------------+----------------+
-- | license_plate |  name  |  phone_number  | passport_number |    receiver    |
-- +---------------+--------+----------------+-----------------+----------------+
-- | 94KL13X       | Bruce  | (367) 555-5533 | 5773159633      | (375) 555-8161 |
-- | 1106N58       | Taylor | (286) 555-6063 | 1988161715      | (676) 555-6554 |
-- | 322W7JE       | Diana  | (770) 555-1861 | 3592750733      | (725) 555-3243 |
-- | 94KL13X       | Bruce  | (367) 555-5533 | 5773159633      | (455) 555-5315 |
-- +---------------+--------+----------------+-----------------+----------------+

-- Relating all the informations I was able to see three suspects
----------------------------------------------------------------------------------


----------------------------------------------------------------------------------
-- Check how the passengers table was created
.schema passengers

-- check the three passport_number
SELECT *
FROM passengers
WHERE passport_number = 5773159633
OR passport_number = 1988161715
OR passport_number = 3592750733;

-- +-----------+-----------------+------+
-- | flight_id | passport_number | seat |
-- +-----------+-----------------+------+
-- | 18        | 3592750733      | 4C   |
-- | 24        | 3592750733      | 2C   |
-- | 36        | 5773159633      | 4A   |
-- | 36        | 1988161715      | 6D   |
-- | 54        | 3592750733      | 6C   |
-- +-----------+-----------------+------+

-- Checking the passengers information I see all three suspects are passenger.
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Check how the flights table was created
.schema flights

SELECT passengers.passport_number, origin_airport_id, destination_airport_id, hour
FROM passengers, flights
WHERE (flights.id = 18 OR flights.id = 24 OR flights.id = 36 OR flights.id = 54)
AND flight_id = flights.id
AND year = 2024
AND month = 7
AND day = 29
AND (passengers.passport_number = 5773159633
OR passengers.passport_number = 1988161715
OR passengers.passport_number = 3592750733);

-- +-----------------+-------------------+------------------------+------+
-- | passport_number | origin_airport_id | destination_airport_id | hour |
-- +-----------------+-------------------+------------------------+------+
-- | 3592750733      | 8                 | 6                      | 16   |
-- | 5773159633      | 8                 | 4                      | 8    |
-- | 1988161715      | 8                 | 4                      | 8    |
-- +-----------------+-------------------+------------------------+------+

-- As the flight will be the earliest, so we can exclude the flight at 16
-- These two passangers are Bruce and Taylor, so now I have to search who they call.

-- Check for who Taylor and Bruce called
SELECT id, name, passport_number
FROM people
WHERE phone_number = '(375) 555-8161'
OR phone_number = '(676) 555-6554'
OR phone_number = '(455) 555-5315';

-- +--------+-----------+-----------------+
-- |   id   |   name    | passport_number |
-- +--------+-----------+-----------------+
-- | 250277 | James     | 2438825627      |
-- | 639344 | Charlotte | 7226911797      |
-- | 864400 | Robin     | NULL            |
-- +--------+-----------+-----------------+

SELECT name FROM people WHERE phone_number = '(375) 555-8161';
-- Bruce called to Robin and Robin doesn't have passport, which can guarantee that he is the accomplice.

----------------------------------------------------------------------------------
-- Check how the airport was created
.schema airports

-- Check the location
SELECT full_name, city
FROM airports
WHERE id = 4;

-- +-------------------+---------------+
-- |     full_name     |     city      |
-- +-------------------+---------------+
-- | LaGuardia Airport | New York City |
-- +-------------------+---------------+
