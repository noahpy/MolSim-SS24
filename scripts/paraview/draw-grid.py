import vtk

def create_box_with_cells(origin, box_size, cell_size, do_cells = True):
    points = vtk.vtkPoints()
    lines = vtk.vtkCellArray()

    # Unpack origin and sizes
    ox, oy, oz = origin
    box_width, box_height, box_depth = box_size
    cell_width, cell_height, cell_depth = cell_size

    # Define the points of the main box
    points.InsertNextPoint(ox, oy, oz)                                      # 0 back-left-bottom
    points.InsertNextPoint(ox + box_width, oy, oz)                          # 1 back-right-bottom
    points.InsertNextPoint(ox + box_width, oy + box_height, oz)             # 2 back-right-top
    points.InsertNextPoint(ox, oy + box_height, oz)                         # 3 back-left-top
    points.InsertNextPoint(ox, oy, oz + box_depth)                          # 4 front-left-bottom
    points.InsertNextPoint(ox + box_width, oy, oz + box_depth)              # 5 front-right-bottom
    points.InsertNextPoint(ox + box_width, oy + box_height, oz + box_depth) # 6 front-right-top
    points.InsertNextPoint(ox, oy + box_height, oz + box_depth)             # 7 front-left-top

    # Define the edges of the main box
    main_edges = [
        (0, 1), (1, 5), (5, 4), (4, 0),  # Bottom edges
        (3, 2), (2, 6), (6, 7), (7, 3),  # Top edges
        (0, 3), (1, 2), (4, 7), (5, 6)   # Vertical edges
    ]

    for edge in main_edges:
        line = vtk.vtkLine()
        line.GetPointIds().SetId(0, edge[0])
        line.GetPointIds().SetId(1, edge[1])
        lines.InsertNextCell(line)

    if do_cells:
        # Define the smaller cells within the main box
        num_cells_x = abs(int(box_width / cell_width))
        num_cells_y = abs(int(box_height / cell_height))
        if box_depth == 0:
            num_cells_z = 1
        else:
            num_cells_z = abs(int(box_depth / cell_depth))

        for i in range(num_cells_x):
            for j in range(num_cells_y):
                for k in range(num_cells_z):
                    cx = ox + i * cell_width
                    cy = oy + j * cell_height
                    cz = oz + k * cell_depth

                    # Define the points of the cell
                    p0 = points.InsertNextPoint(cx, cy, cz)
                    p1 = points.InsertNextPoint(cx + cell_width, cy, cz)
                    p2 = points.InsertNextPoint(cx + cell_width, cy + cell_height, cz)
                    p3 = points.InsertNextPoint(cx, cy + cell_height, cz)
                    p4 = points.InsertNextPoint(cx, cy, cz + cell_depth)
                    p5 = points.InsertNextPoint(cx + cell_width, cy, cz + cell_depth)
                    p6 = points.InsertNextPoint(cx + cell_width, cy + cell_height, cz + cell_depth)
                    p7 = points.InsertNextPoint(cx, cy + cell_height, cz + cell_depth)

                    # Define the edges of the cell
                    cell_edges = [
                        (p0, p1), (p1, p2), (p2, p3), (p3, p0),  # Bottom edges
                        (p4, p5), (p5, p6), (p6, p7), (p7, p4),  # Top edges
                        (p0, p4), (p1, p5), (p2, p6), (p3, p7)   # Vertical edges
                    ]

                    for edge in cell_edges:
                        line = vtk.vtkLine()
                        line.GetPointIds().SetId(0, edge[0])
                        line.GetPointIds().SetId(1, edge[1])
                        lines.InsertNextCell(line)

    # Create the polydata to hold the points and lines
    polydata = vtk.vtkPolyData()
    polydata.SetPoints(points)
    polydata.SetLines(lines)

    return polydata

# Example parameters
origin = (0, 0, 0)
box_size = (180, 90, 0)
cell_size = (3,3,0)
do_cells = True

# Create the box with cells
output_polydata = create_box_with_cells(origin, box_size, cell_size, do_cells=do_cells)

# Set the output
output.ShallowCopy(output_polydata)
