SELECT P.name FROM people P
JOIN directors D ON P.id = D.person_id
JOIN ratings R ON R.movie_id = D.movie_ID
WHERE R.rating >= 9.0;