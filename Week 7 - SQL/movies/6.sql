SELECT AVG(ratings.rating)
FROM movies, ratings
WHERE year = 2012
AND ratings.movie_id = movies.id;
