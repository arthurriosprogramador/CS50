SELECT people.name
FROM movies, people, stars
WHERE movies.title = 'Toy Story'
AND stars.movie_id = movies.id
AND people.id = stars.person_id;
