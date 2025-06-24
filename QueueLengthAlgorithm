class Lane: 
    def _init_(self, name, l_i, n_2i, n_1i, l_inter): 
        self.name = name 
        self.l_i = l_i 
        self.n_2i = n_2i 
        self.n_1i = n_1i 
        self.l_inter = l_inter 
 
    def estimate_lq(self): 
        n_qi = self.n_2i - self.n_1i 
        return self.l_i + (n_qi - 1) * self.l_inter if n_qi > 0 else self.l_i 
 
 
class Intersection: 
    def _init_(self, name, lanes): 
        self.name = name 
        self.lanes = lanes 
 
    def estimate_total_lq(self): 
        return sum(lane.estimate_lq() for lane in self.lanes) 
 
    def assign_green_time(self, total_cycle_time, global_total_lq): 
        self.green_time = (self.estimate_total_lq() / global_total_lq) * total_cycle_time 
 
        for lane in self.lanes: 
            lq = lane.estimate_lq() 
            lane.green_time = (lq / self.estimate_total_lq()) * self.green_time 
 
    def print_signal_plan(self): 
        print(f"\n--- {self.name} ---") 
        print(f"Total Queue Length: {self.estimate_total_lq():.2f} m") 
        print(f"Allocated Green Time: {self.green_time:.2f} s") 
        for lane in self.lanes: 
            print(f"{lane.name} -> Queue Length: {lane.estimate_lq():.2f} m | Green Time: {lane.green_time:.2f} 
s") 
 
 
# Setup intersections 
intersection1 = Intersection("Intersection 1", [ 
    Lane("Lane1", 20, 12, 8, 7), 
    Lane("Lane2", 20, 15, 10, 7), 
    Lane("Lane3", 20, 9, 6, 7), 
    Lane("Lane4", 20, 11, 7, 7), 
]) 
 
intersection2 = Intersection("Intersection 2", [ 
Lane("Lane1", 20, 10, 5, 7), 
Lane("Lane2", 20, 13, 9, 7), 
Lane("Lane3", 20, 7, 4, 7), 
Lane("Lane4", 20, 8, 3, 7), 
]) 
intersection3 = Intersection("Intersection 3", [ 
Lane("Lane1", 20, 6, 4, 7), 
Lane("Lane2", 20, 7, 5, 7), 
Lane("Lane3", 20, 5, 3, 7), 
Lane("Lane4", 20, 4, 2, 7), 
]) 
# Step 1: Compute global queue length across all intersections 
intersections = [intersection1, intersection2, intersection3] 
global_total_lq = sum(intersection.estimate_total_lq() for intersection in intersections) 
# Step 2: Assign and coordinate green time 
total_cycle_time = 120  # seconds 
for intersection in intersections: 
intersection.assign_green_time(total_cycle_time, global_total_lq) 
# Step 3: Print coordinated plan 
print("\n=== Coordinated Traffic Signal Plan ===") 
for intersection in intersections: 
intersection.print_signal_plan()
