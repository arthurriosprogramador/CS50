SELECT DISTINCT movies.title
FROM movies, people AS p1, people AS p2, stars AS s1, stars AS s2
WHERE p1.name = 'Bradley Cooper'
AND p2.name = 'Jennifer Lawrence'
AND s1.person_id = p1.id
AND s2.person_id = p2.id
AND movies.id = s1.movie_id
AND movies.id = s2.movie_id;