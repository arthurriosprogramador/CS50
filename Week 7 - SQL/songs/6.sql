SELECT songs.name FROM artists, songs
WHERE songs.artist_id = artists.id
AND artists.name = 'Post Malone';
