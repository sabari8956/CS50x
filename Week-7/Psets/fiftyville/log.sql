-- take a look at tables
.table

/*
airports              crime_scene_reports   people
atm_transactions      flights               phone_calls
bakery_security_logs  interviews
bank_accounts         passengers
*/



-- first we'll take a look at crime_scene_reports

.schema crime_scene_reports
/*
CREATE TABLE crime_scene_reports (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    street TEXT,
    description TEXT,
    PRIMARY KEY(id)
);
*/



-- seeing the crimes on the day

SELECT description
  FROM crime_scene_reports
 WHERE day = 28
   AND month = 7

/*  Result
| Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
 Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
*/

-- it took place at 10:15

-- 3 interviews took place on the day of that event
-- we will look into it

-- lets have a look at interview table

.schema interviews
/*
CREATE TABLE interviews (
    id INTEGER,
    name TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    transcript TEXT,
    PRIMARY KEY(id)
);
*/



-- Lets see the interviews on the day

SELECT name, transcript
  FROM interviews
 WHERE day = 28
   AND month = 7

/*
+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|  name   |                                                                                                                                                     transcript                                                                                                                                                      |
+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
| Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
| Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
*/


/*  THERE ARE 3 LEADS FROM THIS */

-- FROM RUTH
-- within ten minutes thief g0t into a car and drove away

-- clues we have
-- day 28
-- month 7
-- hour 10
-- minutue 15 > 25 < 35 after .15 and before .35



-- lets see the structure

.schema bakery_security_logs

/*
CREATE TABLE bakery_security_logs (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    activity TEXT,
    license_plate TEXT,
    PRIMARY KEY(id)
);
*/
-- license_plate will be useful
-- we can get name with license plate

-- we have a table people
/*
CREATE TABLE people (
    id INTEGER,
    name TEXT,
    phone_number TEXT,
    passport_number INTEGER,
    license_plate TEXT,
    PRIMARY KEY(id)
);
*/
-- we can join license plate and get the name

SELECT P.name
  FROM people P
  JOIN bakery_security_logs B ON P.license_plate = B.license_plate
 WHERE day = 28
   AND month = 7
   AND hour = 10
   AND minute > 15
   AND minute < 35
   AND activity = 'exit'
 ORDER BY minute;

 /* CURRENT SUSPECTS
 +---------+
|  name   |
+---------+
| Vanessa |
| Bruce   |
| Barry   |
| Luca    |
| Sofia   |
| Iman    |
| Diana   |
| Kelsey  |
+---------+
*/




-- WE GOT THE SUSPECTS FROM THE EXIT VECHICLES

-- NEXT EUGENE - by the ATM on Leggett Street on the day the thief there withdrawing some money

-- CLUES

-- SPOTED ON ATM
-- LOCATION Leggett Street
-- ACTIVITY WITHDRAW


-- ON ATM's WE HAVE atm transactions

-- structure of atm transactions
.schema atm_transactions
/*
CREATE TABLE atm_transactions (
    id INTEGER,
    account_number INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    atm_location TEXT,
    transaction_type TEXT,
    amount INTEGER,
    PRIMARY KEY(id)
);
*/

-- just the account_number isn't  enough


-- AND THERE IS A BANK_ACCOUNT TABLE

-- lets see the struct of bank_accounts

.schema bank_accounts
/*
CREATE TABLE bank_accounts (
    account_number INTEGER,
    person_id INTEGER,
    creation_year INTEGER,
    FOREIGN KEY(person_id) REFERENCES people(id)
);
*/

-- STILL WE GET THE PERSON ID

-- WE NEED TO CONVERT PERSON ID TO NAME




-- LETS NEST THE 3 TABLES AND GET THE NAME

SELECT P.name
  FROM people P
  JOIN bank_accounts B ON P.id = B.person_id
  JOIN atm_transactions T ON B.account_number = T.account_number
 WHERE T.atm_location = 'Leggett Street'
   AND T.day = 28
   AND T.month = 7
 ORDER BY amount DESC;

 /*
 +---------+
|  name   |
+---------+
| Brooke  |
| Taylor  |
| Bruce   |
| Luca    |
| Diana   |
| Benista |
| Kenny   |
| Iman    |
| Kaelyn  |
+---------+
*/


-- CAN REDUCE THE LIST OF SUSPECTS BY INTERSECTing BOTH THE RESULTS

-- suspects from ruth
SELECT P.name
  FROM people P
  JOIN bakery_security_logs B ON P.license_plate = B.license_plate
 WHERE day = 28
   AND month = 7
   AND hour = 10
   AND minute > 15
   AND minute < 35
   AND activity = 'exit'

INTERSECT

-- SUSPECTS FROM EUGENE
SELECT P.name
  FROM people P
  JOIN bank_accounts B ON P.id = B.person_id
  JOIN atm_transactions T ON B.account_number = T.account_number
 WHERE T.atm_location = 'Leggett Street'
   AND T.day = 28
   AND T.month = 7


-- UPDATED SUSPECTS

/*
+--------+
|  name  |
+--------+
| Bruce  |
| Diana  |
| Iman   |
| Luca   |
+--------+
*/




-- LETS GET TO RAYMOND
-- As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
-- In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- CLUES

-- CALL TOOK PLACE FOR LESS THAT 60s
-- THEIF IS FLYING OUT OF FIFTYVILLE TOMORROW
-- FLIGHT ON 29
-- SOMEONE IS HELPING THE THEIF

-- WE NEED SOME INFO ABOUT PHONE CALLS WE'LL SEE WHAT WE HAVE
-- WE HAVE A TABLE ON phone_calls

.schema phone_calls

/*
CREATE TABLE phone_calls (
    id INTEGER,
    caller TEXT,
    receiver TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    duration INTEGER,
    PRIMARY KEY(id)
);
*/
-- LETS SEE WHAT WE GET BACK ON CALLER AND RECEIVER

SELECT caller, receiver
  FROM phone_calls
 WHERE day = 28
   AND month = 7
   AND duration <=60

-- we get back just the numbers
/*
+----------------+----------------+
|     caller     |    receiver    |
+----------------+----------------+
| (130) 555-0289 | (996) 555-8899 |
| (499) 555-9472 | (892) 555-8872 |
| (367) 555-5533 | (375) 555-8161 |
| (609) 555-5876 | (389) 555-5198 |
| (499) 555-9472 | (717) 555-1342 |
| (286) 555-6063 | (676) 555-6554 |
| (770) 555-1861 | (725) 555-3243 |
| (031) 555-6622 | (910) 555-3251 |
| (826) 555-1652 | (066) 555-9701 |
| (338) 555-6650 | (704) 555-2131 |
+----------------+----------------+
*/

-- FROM PEOPLE WE CAN GET GET NAMES OF CALLER AND RECEIVER

SELECT C.name AS Caller , R.name AS Reciver
  FROM phone_calls P
  JOIN people AS C ON P.caller = C.phone_number
  JOIN people AS R ON P.receiver = R.phone_number
 WHERE P.day = 28
   AND P.month = 7
   AND P.duration <= 60;

/*
+---------+------------+
| Caller  |  Reciver   |
+---------+------------+
| Sofia   | Jack       |
| Kelsey  | Larry      |
| Bruce   | Robin      |
| Kathryn | Luca       |
| Kelsey  | Melissa    |
| Taylor  | James      |
| Diana   | Philip     |
| Carina  | Jacqueline |
| Kenny   | Doris      |
| Benista | Anna       |
+---------+------------+
*/


-- to combine the suspected name with this call and get the calls of the suspects

SELECT result1.Caller, result1.Reciver
FROM (
  SELECT C.name AS Caller, R.name AS Reciver
  FROM phone_calls P
  JOIN people AS C ON P.caller = C.phone_number
  JOIN people AS R ON P.receiver = R.phone_number
  WHERE P.day = 28
    AND P.month = 7
    AND P.duration <= 60
) AS result1
JOIN (
  SELECT P.name
  FROM people P
  JOIN bakery_security_logs B ON P.license_plate = B.license_plate
  WHERE B.day = 28
    AND B.month = 7
    AND B.hour = 10
    AND B.minute >= 15
    AND B.minute <= 35
    AND B.activity = 'exit'
) AS result2
ON result1.Caller = result2.name;

/*
+--------+---------+
| Caller | Reciver |
+--------+---------+
| Sofia  | Jack    |
| Kelsey | Larry   |
| Bruce  | Robin   |
| Kelsey | Melissa |
| Taylor | James   |
| Diana  | Philip  |
+--------+---------+
*/


-- LETS SEE THE FLIGHT PASSENGERS

.schema passengers

/*
CREATE TABLE passengers (
    flight_id INTEGER,
    passport_number INTEGER,
    seat TEXT,
    FOREIGN KEY(flight_id) REFERENCES flights(id)
);
*/

-- we need the passport number
-- we can get it from people

-- suspects flight id

SELECT F.flight_id
  FROM passengers F
  JOIN people P ON F.passport_number = P.passport_number
 WHERE P.name IN ('Bruce', 'Diana', 'Iman', 'Luca')

/*
+-----------+
| flight_id |
+-----------+
| 26        |
| 36        |
| 11        |
| 36        |
| 48        |
| 18        |
| 24        |
| 54        |
| 36        |
+-----------+
*/

-- we will see the flights with flight id


-- struct of flights
/*
CREATE TABLE flights (
    id INTEGER,
    origin_airport_id INTEGER,
    destination_airport_id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    PRIMARY KEY(id),
    FOREIGN KEY(origin_airport_id) REFERENCES airports(id),
    FOREIGN KEY(destination_airport_id) REFERENCES airports(id)
);
*/

-- clues

-- WE HAVE IDs
-- NEED TO GET ID OF ORGIN_AIRPORT AKA FIFTYVILLE AIRPORT ID
-- DAY 29
-- MONTH 7
-- take flight by eariest
select * from airports

-- |8|CSF|Fiftyville Regional Airport|Fiftyville|
-- 8 is the id of orgin airport

SELECT F.destination_airport_id, M.name
  FROM flights F
  JOIN passengers P ON F.id = P.flight_id
  JOIN people M on P.passport_number = M.passport_number
 WHERE M.name IN ('Bruce', 'Diana', 'Iman', 'Luca')
   AND F.day = 29
   AND F.month = 7
   AND F.origin_airport_id = 8
 ORDER BY hour;

/*
+------------------------+--------+
| destination_airport_id |  name  |
+------------------------+--------+
| 4                      | Bruce  |
| 4                      | Taylor |
| 4                      | Luca   |
| 6                      | Diana  |
+------------------------+--------+
*/
-- the theif wants to take flight as soon as possible but flight to airport 6 is later evening
-- so it isnt diana
-- new suspects
/*
+------------------------+--------+
| destination_airport_id |  name  |
+------------------------+--------+
| 4                      | Bruce  |
| 4                      | Taylor |
| 4                      | Luca   |
+------------------------+--------+
*/

-- so its confirm that its airport id 4

select * from airports where id = 4
/*
+----+--------------+-------------------+---------------+
| id | abbreviation |     full_name     |     city      |
+----+--------------+-------------------+---------------+
| 4  | LGA          | LaGuardia Airport | New York City |
+----+--------------+-------------------+---------------+
*/

-- its New York City
-- lets combine with the caller
SELECT result1.Caller
FROM (
  SELECT C.name AS Caller, R.name AS Receiver
  FROM phone_calls P
  JOIN people AS C ON P.caller = C.phone_number
  JOIN people AS R ON P.receiver = R.phone_number
  WHERE P.day = 28
    AND P.month = 7
    AND P.duration <= 60
) AS result1
JOIN (
  SELECT P.name
  FROM people P
  JOIN bakery_security_logs B ON P.license_plate = B.license_plate
  WHERE B.day = 28
    AND B.month = 7
    AND B.hour = 10
    AND B.minute >= 15
    AND B.minute <= 35
    AND B.activity = 'exit'
) AS result2
ON result1.Caller = result2.name
INTERSECT
SELECT M.name
FROM flights F
JOIN passengers P ON F.id = P.flight_id
JOIN people M ON P.passport_number = M.passport_number
WHERE M.name IN ('Bruce',  'Iman', 'Luca')
  AND F.day = 29
  AND F.month = 7
  AND F.origin_airport_id = 8


/*
+--------+
| Caller |
+--------+
| Bruce  |
+--------+
*/


-- the theif is Bruce
/*
+--------+---------+
| Caller | Reciver |
+--------+---------+
| Bruce  | Robin   |
+--------+---------+
*/

-- AND THE ONE HELPED HIM IS ROBIN

-- The THIEF is: Bruce
-- The city the thief ESCAPED TO: New York City
-- The ACCOMPLICE is: Robin


-- CASE CLOSED

