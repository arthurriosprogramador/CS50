SELECT DISTINCT people.name
FROM movies, people, directors, ratings
WHERE ratings.movie_id = movies.id
AND ratings.rating >= 9.0
AND people.id = directors.person_id
AND directors.movie_id = movies.id;

