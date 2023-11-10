SELECT P.name FROM people P
JOIN stars S ON S.person_id = P.id
JOIN movies M ON M.id = S.movie_id
WHERE M.year = 2004
ORDER BY P.birth;