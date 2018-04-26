function [ h, B, P ] = plotplane(I, bounds, a, b, s, n)

assert(a > 0 && a < 4);
assert(b > 0 && b < 4);
assert(a ~= b);

c = 4 - (a - 1) - (b - 1);

bmin = bounds(:, 1);
bmax = bounds(:, 2);
brng = bmax - bmin;

box = {};
box.size = zeros(3, 1);

box.size(a) = brng(a) / size(I, 1);
box.size(b) = brng(b) / size(I, 2);
box.size(c) = min(box.size(a), box.size(b));
origin = bmin + box.size / 2;
origin(c) = bmin(c) - box.size(c) / 2;


tcount = sum(I(:) > 0);
P = zeros(3, n * tcount);

if s > 0
    origin(c) = bmax(c) + box.size(c) / 2;
end

if s > 0
    P(c, :) = bmax(c);
else
    P(c, :) = bmin(c);
end

box.center = origin;
box.textured = false;

B(length(I)) = box;
h = gobjects(size(I, 1), size(I, 2), 7);
k = 1;
pi = 0;

for i = 1 : size(I, 1)
    
    box.center(a) = origin(a) + (i - 1) * box.size(a);
    
    for j = 1 : size(I, 2)
        
        box.center(b) = origin(b) + (j - 1) * box.size(b);
        box.textured = I(i, j) > 0;
        h(i, j, :) = plotbox(box);
        B(k) = box;
        k = k + 1;
        
        if box.textured
            
            for s = 1 : n
                
                P(a, pi + s) = box.center(a) + (rand - 0.5) * box.size(a);
                P(b, pi + s) = box.center(b) + (rand - 0.5) * box.size(b);
                
            end
            
            pi = pi + n;
            
        end
        
    end
end

plot3(P(1, :), P(2, :), P(3, :), 'g.');

end