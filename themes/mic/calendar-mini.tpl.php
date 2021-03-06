<?php
// $Id: calendar-mini.tpl.php,v 1.7 2010/12/21 13:41:24 karens Exp $
/**
 * @file
 * Template to display a view as a mini calendar month.
 * 
 * @see template_preprocess_calendar_mini.
 *
 * $day_names: An array of the day of week names for the table header.
 * $rows: An array of data for each day of the week.
 * $view: The view.
 * $min_date_formatted: The minimum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * $max_date_formatted: The maximum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * 
 * $show_title: If the title should be displayed. Normally false since the title is incorporated
 *   into the navigation, but sometimes needed, like in the year view of mini calendars.
 * 
 */
//dsm('Display: '. $display_type .': '. $min_date_formatted .' to '. $max_date_formatted);
?>
<div class="calendar-calendar">
	<div class="month-view">
		<?php if ($view->date_info->show_title): ?>
			<?php print theme('date_navigation', array('view' => $view)); ?>
		<?php endif; ?> 

		<div class="calendar-widget-wrapper">
			
			<?php foreach($day_names as $day): ?>
				<div class="day-name">
					<?php echo $day['data']; ?>
				</div>
			<?php endforeach; ?>
			
			<div class="clear"></div>
			
		    <?php foreach ((array) $rows as $row): ?>
				<div class="row-wrapper">
					<?php foreach ($row as $cell): ?>
						<div class="left <?php print $cell['class']; ?> <?php print $cell['id']; ?>">
							<?php print $cell['data']; ?>
						</div>
					<?php endforeach; ?>
					<div class="clear"></div>
				</div>
				
		    <?php endforeach; ?>
		    
		</div>
	</div>
</div>
