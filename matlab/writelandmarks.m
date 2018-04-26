function writelandmarks(file, landmarks)

n = size(landmarks, 2);

for i = 1 : n
    
    fprintf(file, '%f,%f,%f\n', ...
        landmarks(1, i), landmarks(2, i), landmarks(3, i));
    
end

end
