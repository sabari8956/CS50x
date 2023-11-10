SELECT DISTINCT people.name
FROM people
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
JOIN stars AS stars2 ON movies.id = stars2.movie_id
JOIN people AS people2 ON stars2.person_id = people2.id
WHERE people2.name = 'Kevin Bacon' AND people2.birth = 1958 AND people.name != 'Kevin Bacon';