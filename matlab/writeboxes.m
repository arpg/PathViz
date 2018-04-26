function writeboxes(file, boxes)

n = length(boxes);

for i = 1 : n

    box = boxes(i);
    
    fprintf(file, '%f,%f,%f,%f,%f,%f,%d\n', ...
        box.center(1), box.center(2), box.center(3), ...
        box.size(1), box.size(2), box.size(3), ...
        box.textured);
end

end