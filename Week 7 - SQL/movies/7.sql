SELECT movies.title, ratings.rating
FROM movies, ratings
WHERE movies.year = 2010
AND ratings.movie_id = movies.id
ORDER BY ratings.rating DESC, movies.title ASC;
