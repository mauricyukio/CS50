-- Get more details about the crime by knowing the date and street it took place on
SELECT description FROM crime_scene_reports WHERE year = 2021 AND month = 7 AND day = 28 AND street LIKE "Humphrey%";

-- Relevant Results:
-- Theft of the CS50 duck took place at *10:15am* at the Humphrey Street bakery.
-- Interviews were conducted today with *three witnesses* who were present at the time – each of their interview transcripts mentions the *bakery*.

-- Look at the bakery security logs for hour 10 for car activity. Might be relevant later
SELECT activity, license_plate, minute FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10;
-- +----------+---------------+--------+
-- | activity | license_plate | minute |
-- +----------+---------------+--------+
-- | entrance | R3G7486       | 8      |
-- | entrance | 13FNH73       | 14     |
-- | exit     | 5P2BI95       | 16     |
-- | exit     | 94KL13X       | 18     |
-- | exit     | 6P58WS2       | 18     |
-- | exit     | 4328GD8       | 19     |
-- | exit     | G412CB7       | 20     |
-- | exit     | L93JTIZ       | 21     |
-- | exit     | 322W7JE       | 23     |
-- | exit     | 0NTHK55       | 23     |
-- | exit     | 1106N58       | 35     |
-- | entrance | NRYN856       | 42     |
-- | entrance | WD5M8I6       | 44     |
-- | entrance | V47T75I       | 55     |
-- +----------+---------------+--------+

-- Look for the 3 interviews that mention the bakery that day, as well as the name of the interviewed people
SELECT name, transcript FROM interviews WHERE year = 2021 AND month = 7 AND day = 28;
-- Relevant Results:

-- | Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.

-- | Eugene  | I don't know the thief's name, but it was someone I recognized.
-- Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.

-- | Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
-- In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Look at the withdrawals that happened that day on the Leggett Street ATM, and the corresponding account number
SELECT account_number, atm_location, transaction_type, amount FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28;
SELECT account_number, amount FROM atm_transactions
    WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";
-- +----------------+--------+
-- | account_number | amount |
-- +----------------+--------+
-- | 28500762       | 48     |
-- | 28296815       | 20     |
-- | 76054385       | 60     |
-- | 49610011       | 50     |
-- | 16153065       | 80     |
-- | 25506511       | 20     |
-- | 81061156       | 30     |
-- | 26013199       | 35     |
-- +----------------+--------+

-- Look at the phone call logs that happened that day, that lasted for less than a minute
SELECT caller, receiver, duration FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration <= 60;
-- +----------------+----------------+----------+
-- |     caller     |    receiver    | duration |
-- +----------------+----------------+----------+
-- | (130) 555-0289 | (996) 555-8899 | 51       |
-- | (499) 555-9472 | (892) 555-8872 | 36       |
-- | (367) 555-5533 | (375) 555-8161 | 45       |
-- | (609) 555-5876 | (389) 555-5198 | 60       |
-- | (499) 555-9472 | (717) 555-1342 | 50       |
-- | (286) 555-6063 | (676) 555-6554 | 43       |
-- | (770) 555-1861 | (725) 555-3243 | 49       |
-- | (031) 555-6622 | (910) 555-3251 | 38       |
-- | (826) 555-1652 | (066) 555-9701 | 55       |
-- | (338) 555-6650 | (704) 555-2131 | 54       |
-- +----------------+----------------+----------+\

-- With all this information, we can narrow down our list of suspects
SELECT * FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id WHERE
    people.phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration <= 60) AND
    people.license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10) AND
    bank_accounts.account_number IN (SELECT account_number FROM atm_transactions
        WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw");
-- +--------+--------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- |   id   |  name  |  phone_number  | passport_number | license_plate | account_number | person_id | creation_year |
-- +--------+--------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       | 49610011       | 686048    | 2010          |
-- | 514354 | Diana  | (770) 555-1861 | 3592750733      | 322W7JE       | 26013199       | 514354    | 2012          |
-- | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       | 76054385       | 449774    | 2015          |
-- +--------+--------+----------------+-----------------+---------------+----------------+-----------+---------------+
--Apparently we need a little more data to single the thief out

-- Let's find out the destination airports from Fiftyville for the day 29, and to which city the first flight of the day went
SELECT flights.id, airports.city, flights.hour, flights.minute FROM flights JOIN airports ON airports.id = flights.destination_airport_id
    WHERE origin_airport_id = (SELECT id FROM airports WHERE city LIKE "Fiftyville") AND year = 2021 AND month = 7 AND day = 29 ORDER BY hour ASC;
-- +----+---------------+------+--------+
-- | id |     city      | hour | minute |
-- +----+---------------+------+--------+
-- | 36 | New York City | 8    | 20     |
-- | 43 | Chicago       | 9    | 30     |
-- | 23 | San Francisco | 12   | 15     |
-- | 53 | Tokyo         | 15   | 20     |
-- | 18 | Boston        | 16   | 0      |
-- +----+---------------+------+--------+
-- Looks like the thief has escaped to New Yorkon flight 36! Let's catch him!

-- These are all the passengers' passport numbers who went to Dallas on said flight:
SELECT passport_number FROM passengers WHERE flight_id = 36;
-- +-----------------+
-- | passport_number |
-- +-----------------+
-- | 7214083635      |
-- | 1695452385      |
-- | 5773159633      |
-- | 1540955065      |
-- | 8294398571      |
-- | 1988161715      |
-- | 9878712108      |
-- | 8496433585      |
-- +-----------------+

-- That was the last piece of information we needed. Now we can plug it in our previous query
SELECT * FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id WHERE
    people.phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration <= 60) AND
    people.license_plate IN (SELECT license_plate FROM bakery_security_logs
        WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND activity = "exit") AND
    bank_accounts.account_number IN (SELECT account_number FROM atm_transactions
        WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw") AND
    people.passport_number IN (SELECT passport_number FROM passengers WHERE flight_id = 36);
-- +--------+--------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- |   id   |  name  |  phone_number  | passport_number | license_plate | account_number | person_id | creation_year |
-- +--------+--------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- | 686048 | Bruce  | (367) 555-5533 | 5773159633      | 94KL13X       | 49610011       | 686048    | 2010          |
-- | 449774 | Taylor | (286) 555-6063 | 1988161715      | 1106N58       | 76054385       | 449774    | 2015          |
-- +--------+--------+----------------+-----------------+---------------+----------------+-----------+---------------+
-- No way there are still two people left! I'm upset and happy at the same time. We've gotta dig deeper

-- Let's see who these two are calling the most, maybe we can find their accomplice:
SELECT * FROM phone_calls WHERE caller = "(367) 555-5533";
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | id  |     caller     |    receiver    | year | month | day | duration |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | 9   | (367) 555-5533 | (113) 555-7544 | 2021 | 7     | 25  | 469      |
-- | 104 | (367) 555-5533 | (238) 555-5554 | 2021 | 7     | 26  | 84       |
-- | 122 | (367) 555-5533 | (660) 555-3095 | 2021 | 7     | 26  | 399      |
-- | 133 | (367) 555-5533 | (286) 555-0131 | 2021 | 7     | 26  | 444      |
-- | 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       |
-- | 236 | (367) 555-5533 | (344) 555-9601 | 2021 | 7     | 28  | 120      |
-- | 245 | (367) 555-5533 | (022) 555-4052 | 2021 | 7     | 28  | 241      |
-- | 285 | (367) 555-5533 | (704) 555-5790 | 2021 | 7     | 28  | 75       |
-- | 395 | (367) 555-5533 | (455) 555-5315 | 2021 | 7     | 30  | 31       |
-- | 418 | (367) 555-5533 | (841) 555-3728 | 2021 | 7     | 30  | 511      |
-- | 488 | (367) 555-5533 | (696) 555-9195 | 2021 | 7     | 31  | 261      |
-- +-----+----------------+----------------+------+-------+-----+----------+
SELECT * FROM phone_calls WHERE caller = "(286) 555-6063";
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | id  |     caller     |    receiver    | year | month | day | duration |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- | 46  | (286) 555-6063 | (789) 555-8833 | 2021 | 7     | 25  | 125      |
-- | 254 | (286) 555-6063 | (676) 555-6554 | 2021 | 7     | 28  | 43       |
-- | 284 | (286) 555-6063 | (310) 555-8568 | 2021 | 7     | 28  | 235      |
-- | 451 | (286) 555-6063 | (344) 555-9601 | 2021 | 7     | 30  | 154      |
-- +-----+----------------+----------------+------+-------+-----+----------+
-- Doesn't seem to help much

-- Let's look at their ATM transactions. Maybe one of them ended up using an ATM machine in NYC?
SELECT * FROM atm_transactions JOIN bank_accounts
    ON atm_transactions.account_number = bank_accounts.account_number WHERE bank_accounts.account_number = 49610011;
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+----------------+-----------+---------------+
-- | id  | account_number | year | month | day |  atm_location  | transaction_type | amount | account_number | person_id | creation_year |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+----------------+-----------+---------------+
-- | 39  | 49610011       | 2021 | 7     | 26  | Leggett Street | withdraw         | 10     | 49610011       | 686048    | 2010          |
-- | 267 | 49610011       | 2021 | 7     | 28  | Leggett Street | withdraw         | 50     | 49610011       | 686048    | 2010          |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+----------------+-----------+---------------+
SELECT * FROM atm_transactions JOIN bank_accounts
    ON atm_transactions.account_number = bank_accounts.account_number WHERE bank_accounts.account_number = 76054385;
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+----------------+-----------+---------------+
-- | id  | account_number | year | month | day |  atm_location  | transaction_type | amount | account_number | person_id | creation_year |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+----------------+-----------+---------------+
-- | 266 | 76054385       | 2021 | 7     | 28  | Leggett Street | withdraw         | 60     | 76054385       | 449774    | 2015          |
-- +-----+----------------+------+-------+-----+----------------+------------------+--------+----------------+-----------+---------------+
-- Also not helpful at all.

-- Can we really trust the interviews? Are the three of them reliable witnesses and also not the accomplice?
SELECT * FROM people WHERE name IN ("Eugene", "Ruth", "Raymond");
-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 280744 | Eugene  | (666) 555-5774 | 9584465633      | 47592FJ       |
-- | 430845 | Ruth    | (772) 555-5770 |                 | HZB4129       |
-- | 937274 | Raymond | (125) 555-8030 |                 | Y18DLY3       |
-- +--------+---------+----------------+-----------------+---------------+

-- Who were our two suspects calling after they stole the CS50 duck? One of them must be the accomplice
SELECT * FROM people WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND caller = "(367) 555-5533");
-- +--------+---------+----------------+-----------------+---------------+
-- |   id   |  name   |  phone_number  | passport_number | license_plate |
-- +--------+---------+----------------+-----------------+---------------+
-- | 315221 | Gregory | (022) 555-4052 | 3355598951      | V4C670D       |
-- | 652398 | Carl    | (704) 555-5790 | 7771405611      | 81MZ921       |
-- | 864400 | Robin   | (375) 555-8161 |                 | 4V16VO0       |
-- | 985497 | Deborah | (344) 555-9601 | 8714200946      | 10I5658       |
-- +--------+---------+----------------+-----------------+---------------+
SELECT * FROM people WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND caller = "(286) 555-6063");
-- +--------+--------+----------------+-----------------+---------------+
-- |   id   |  name  |  phone_number  | passport_number | license_plate |
-- +--------+--------+----------------+-----------------+---------------+
-- | 250277 | James  | (676) 555-6554 | 2438825627      | Q13SVG6       |
-- | 994229 | Angela | (310) 555-8568 | 9920757687      | SS458D7       |
-- +--------+--------+----------------+-----------------+---------------+

-- Let's look at these 6 people's bank accounts and ATM transactions
SELECT * FROM atm_transactions WHERE account_number IN (
    SELECT account_number FROM bank_accounts WHERE person_id IN (
        SELECT id FROM people WHERE phone_number IN (
            SELECT receiver FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND caller = "(367) 555-5533")));
-- +------+----------------+------+-------+-----+----------------------+------------------+--------+
-- |  id  | account_number | year | month | day |     atm_location     | transaction_type | amount |
-- +------+----------------+------+-------+-----+----------------------+------------------+--------+
-- | 104  | 94751264       | 2021 | 7     | 26  | Daboin Sanchez Drive | withdraw         | 25     |
-- | 242  | 94751264       | 2021 | 7     | 27  | Carvalho Road        | deposit          | 55     |
-- | 417  | 94751264       | 2021 | 7     | 29  | Blumberg Boulevard   | deposit          | 90     |
-- | 585  | 94751264       | 2021 | 7     | 30  | Daboin Sanchez Drive | deposit          | 10     |
-- | 652  | 94751264       | 2021 | 7     | 30  | Leggett Street       | withdraw         | 10     |
-- | 671  | 94751264       | 2021 | 7     | 30  | Humphrey Lane        | deposit          | 15     |
-- | 822  | 94751264       | 2021 | 7     | 31  | Carvalho Road        | deposit          | 100    |
-- | 901  | 94751264       | 2021 | 7     | 31  | Carvalho Road        | withdraw         | 35     |
-- | 1103 | 94751264       | 2021 | 8     | 1   | Daboin Sanchez Drive | deposit          | 40     |
-- | 1121 | 94751264       | 2021 | 8     | 1   | Blumberg Boulevard   | withdraw         | 5      |
-- | 1229 | 94751264       | 2021 | 8     | 1   | Humphrey Lane        | withdraw         | 40     |
-- +------+----------------+------+-------+-----+----------------------+------------------+--------+

SELECT * FROM atm_transactions WHERE account_number IN (
    SELECT account_number FROM bank_accounts WHERE person_id IN (
        SELECT id FROM people WHERE phone_number IN (
            SELECT receiver FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND caller = "(286) 555-6063")));
-- Do 5 of them not have bank accounts or ATM transactions?
SELECT account_number FROM bank_accounts WHERE person_id IN (
    SELECT id FROM people WHERE phone_number IN (
        SELECT receiver FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND caller = "(367) 555-5533"));
-- +----------------+
-- | account_number |
-- +----------------+
-- | 94751264       |
-- +----------------+
-- Apparenty only one people out of those 5 has a bank account. Which of them?
SELECT * FROM people WHERE id = (SELECT person_id FROM bank_accounts WHERE account_number = 94751264);
-- +--------+-------+----------------+-----------------+---------------+
-- |   id   | name  |  phone_number  | passport_number | license_plate |
-- +--------+-------+----------------+-----------------+---------------+
-- | 864400 | Robin | (375) 555-8161 |                 | 4V16VO0       |
-- +--------+-------+----------------+-----------------+---------------+
-- It is Robin, who has no passport. I might be overthinking this. I really think I should start it over to check if I did everything right



-- Let's tackle the interviews one at a time and keep a list of suspects:

-- Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
SELECT name FROM people
    JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
    WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25;
-- +---------+
-- |  name   |
-- +---------+
-- | Vanessa |
-- | Bruce   |
-- | Barry   |
-- | Luca    |
-- | Sofia   |
-- | Iman    |
-- | Diana   |
-- | Kelsey  |
-- +---------+

-- I don't know the thief's name, but it was someone I recognized.
-- Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
SELECT name FROM people
    JOIN bank_accounts ON people.id = bank_accounts.person_id
    JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
    WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";
-- +---------+
-- |  name   |
-- +---------+
-- | Bruce   |
-- | Diana   |
-- | Brooke  |
-- | Kenny   |
-- | Iman    |
-- | Luca    |
-- | Taylor  |
-- | Benista |
-- +---------+

-- As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
SELECT name FROM people
    JOIN phone_calls ON people.phone_number = phone_calls.caller
    WHERE duration <= 60 AND year = 2021 AND month = 7 AND day = 28;
-- +---------+
-- |  name   |
-- +---------+
-- | Sofia   |
-- | Kelsey  |
-- | Bruce   |
-- | Kathryn |
-- | Kelsey  |
-- | Taylor  |
-- | Diana   |
-- | Carina  |
-- | Kenny   |
-- | Benista |
-- +---------+

-- In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.
SELECT name FROM people
    JOIN passengers ON people.passport_number = passengers.passport_number
    JOIN flights ON passengers.flight_id = flights.id
    WHERE flights.id = (
        SELECT id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 AND origin_airport_id = (
            SELECT id FROM airports WHERE city = "Fiftyville") ORDER BY hour, minute ASC LIMIT 1);
-- +--------+
-- |  name  |
-- +--------+
-- | Kenny  |
-- | Sofia  |
-- | Taylor |
-- | Luca   |
-- | Kelsey |
-- | Edward |
-- | Bruce  |
-- | Doris  |
-- +--------+

-- By intersecting these four suspects tables, we should be able to narrow them down
SELECT name FROM people
    JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
    WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25
INTERSECT
SELECT name FROM people
    JOIN bank_accounts ON people.id = bank_accounts.person_id
    JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
    WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"
INTERSECT
SELECT name FROM people
    JOIN phone_calls ON people.phone_number = phone_calls.caller
    WHERE duration <= 60 AND year = 2021 AND month = 7 AND day = 28
INTERSECT
SELECT name FROM people
    JOIN passengers ON people.passport_number = passengers.passport_number
    JOIN flights ON passengers.flight_id = flights.id
    WHERE flights.id = (
        SELECT id FROM flights WHERE year = 2021 AND month = 7 AND day = 29 AND origin_airport_id = (
            SELECT id FROM airports WHERE city = "Fiftyville") ORDER BY hour, minute ASC LIMIT 1);
-- +-------+
-- | name  |
-- +-------+
-- | Bruce |
-- +-------+
-- So, Bruce is definitely our thief this time! However, before I proceed, I want to investigate why I was getting two people (Bruce and Taylor)
-- on the first time. I have a suspicion already that it may be because I was not being so precise with the time the car left the bakery, as I read this
-- "Sometime within ten minutes of the theft" as being imprecise and therefore unreliable information, and then concluding that
-- I should take it with a grain of salt. To double-check:
SELECT hour, minute FROM bakery_security_logs
    JOIN people ON bakery_security_logs.license_plate = people.license_plate
    WHERE year = 2021 AND month = 7 AND day = 28 AND activity = "exit" AND name = "Taylor";
-- +------+--------+
-- | hour | minute |
-- +------+--------+
-- | 10   | 35     |
-- +------+--------+
-- Indeed. As excpected, Taylor's time of exiting the bakery is just slightly out of the range of minutes I should have been considering since the beggining.
-- And I was definitely overthinking it all. Now that this is out of the way, let's move on to the rest of the problem, aka the accomplice

SELECT name FROM people
    JOIN phone_calls ON people.phone_number = phone_calls.receiver
    WHERE year = 2021 AND month = 7 AND day = 28 AND duration <= 60 AND caller = (
        SELECT phone_number FROM people WHERE name = "Bruce");
-- +-------+
-- | name  |
-- +-------+
-- | Robin |
-- +-------+
-- And it was Robin, who has no passport, all along! I should have known it!

-- To finish it off, and to practice a little more, I will find New York City again with a single query, for consistency sake:
SELECT city FROM airports
    JOIN flights ON airports.id = flights.destination_airport_id
    WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville") AND year = 2021 AND month = 7 AND day = 29
    ORDER BY hour, minute ASC LIMIT 1;
-- +---------------+
-- |     city      |
-- +---------------+
-- | New York City |
-- +---------------+
-- And we're done! In case anyone ever reads this, I had a great time doing this exercise, I loved it, if you couldn't tell by the
-- sheer size of this thing. And most importantly, in combination with week7/movies, it forced me to really understand SQL queries.
-- I had never heard of SQL before and I finished this so confident and proud. Congratulations and many thanks, guys, for the amazing
-- educational experience CS50 is. I still can't believe this exists and is free. I'm so grateful I found it. Cheers!