function h = plotbox(box)

nnn = box.center - box.size / 2 + 1E-4;
xxx = box.center + box.size / 2 - 1E-4;

nnx = [ nnn(1) ; nnn(2) ; xxx(3) ];
nxn = [ nnn(1) ; xxx(2) ; nnn(3) ];
nxx = [ nnn(1) ; xxx(2) ; xxx(3) ];
xnn = [ xxx(1) ; nnn(2) ; nnn(3) ];
xnx = [ xxx(1) ; nnn(2) ; xxx(3) ];
xxn = [ xxx(1) ; xxx(2) ; nnn(3) ];

if box.textured
    
    color = 'red';
else
    color = 'white';
end

h = gobjects(7, 1);

P = [ nnn nnx nxx nxn nnn xnn xnx ...
      nnx nxx xxx xnx xnn xxn xxx ...
      nxx nxn xxn nxn nnn ];

h(1) = plot3(P(1, :), P(2, :), P(3, :), '-', 'Color', [ 0.25 0.25 0.25 ]);

P = [ nnn nnx nxx nxn nnn ];
h(2) = fill3(P(1, :), P(2, :), P(3, :), color, 'LineStyle', 'none');

P = [ nnn nnx xnx xnn ];
h(3) = fill3(P(1, :), P(2, :), P(3, :), color, 'LineStyle', 'none');

P = [ nnn nxn xxn xnn nnn ];
h(4) = fill3(P(1, :), P(2, :), P(3, :), color, 'LineStyle', 'none');

P = [ xxx nxx nnx xnx xxx ];
h(5) = fill3(P(1, :), P(2, :), P(3, :), color, 'LineStyle', 'none');

P = [ xxx xxn xnn xnx xxx ];
h(6) = fill3(P(1, :), P(2, :), P(3, :), color, 'LineStyle', 'none');

P = [ xxx nxx nxn xxn xxx ];
h(7) = fill3(P(1, :), P(2, :), P(3, :), color, 'LineStyle', 'none');

alpha(h(2:7), 0.25); 

end
