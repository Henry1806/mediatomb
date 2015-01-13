function init() {
	$(document).foundation();
	console.log("ready!");

	$.ajaxSetup({
		'url': '/content/interface',
		'dataType': 'json'
	});

	get_session();
}

function get_session() {
	$.ajax({
		'data': {
			'req_type': 'auth',
			'action': 'get_sid'
		}}).done(function(data) {
			console.log("done!");
			window.SID = data['sid'];
			main();
		});
}

function main() {
	console.log(SID);

	// Bind stuff
	$('#containers').on('click', 'li', on_container_click);

	// Inital Load
	get_containers(0);
}

function on_container_click(event) {
	event.stopPropagation();
	event.preventDefault();

	var that = $(this);
	var id = that.data('id');

	var $container = that.children('ul.container');

	if (!$container.hasClass("expanded")) {
		get_containers(id, $container);
	} else {
		$container.toggle();
	}


	get_items(id);
}

function get_containers(parent_id, parent_menu_item) {
	$.ajax({
		'data': {
			'req_type': 'containers',
			'action': 'get_config',
			'parent_id': parent_id,
			'sid': SID
		}}).done(function(data) {
			var add_new_level = (typeof parent_menu_item != "undefined");
			var append_target = add_new_level ? parent_menu_item : $('#containers');

			append_target.addClass("expanded");
			
			console.log("append_target: ", append_target, parent_menu_item);

			console.log("Got containers!");
			var containers = data.containers.container;
			for (var i = 0; i < containers.length; i++) {
				console.log(containers[i].title);
				
				var link = $("<li/>");
				link.append($('<img class="padright" src="http://placehold.it/30.png">'))
					.data("id", containers[i]['id'])
					.append(containers[i]['title']);
				var new_level = $('<ul/>').addClass('container');
				link.append(new_level);

				append_target.append(link);
			}
		});
}

function get_items(parent_id) {
	$.ajax({
		'data': {
			'req_type': 'items',
			'action': 'get_config',
			'parent_id': parent_id,
			'sid': SID
		}}).done(function(data) {
			var items = data.items.item;
			console.log(items);
			var list = $('#items').empty();
			for (var i = 0; i < items.length; i++) {
				list.append(render_item_row(items[i]));
			}
		});
}

function render_item_row(item) {
	var row = $('<li/>')
		.append($('<img class="padright"/>').attr("src", item.art).height("50px"))
		.append(item.title)
		.addClass("item");
	return row;
}

$(document).ready(function() {
	init();
});