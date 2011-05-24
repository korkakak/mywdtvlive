<?php
require_once('../security.php');
$isLanReq = isLanRequest();
if($isLanReq === false){
       header("HTTP/1.0 403 Forbidden");
       echo "403 Forbidden";
       return;
}
if (isset($_GET['entry_id'])) {
  $get_entry_id = $_GET['entry_id'];
}
if(isset($_GET['lang_id'])){
	$get_lang_id=$_GET['lang_id'];
}
if(isset($_COOKIE['online'])){

	$get_online_status= $_COOKIE['online'];
}

?>

<?php
try {
    $dbh = new PDO("sqlite:page.db");
	
			$sql = "SELECT * FROM web_top_tag WHERE entry_id =" .$get_entry_id ." AND lang_id=".$get_lang_id. " ORDER BY seq_id ASC";
			
			echo '<div class="tabs">';
			echo "<ul>";
			
			$j=0;
			 
			foreach ($dbh->query($sql) as $row)
			{
				
				if($get_entry_id==2){
					echo  '<li><a id=\'remote_btn\'  href='.$row['tag_url'] .' class="has_separator"> '. $row["tag_name"] . '</a></li>';
					
				}else{
					if($get_entry_id==4 && $get_online_status==1){
						if($j==1 || $j==2){
							echo  '<li><a href='.$row['tag_url'] .' class="has_separator"> '.$row['tag_name'] . '</a></li>';
						}
						
							
					}else{
					
						echo  '<li><a href='.$row['tag_url'] .' class="has_separator"> '.$row['tag_name'] . '</a></li>';
					}
				}
				
				$j++;
			
				
				
			}
			
			
			
			
			echo "</ul>";
			echo "</div>";
			
			$dbh = NULL;
			
	
    }
	catch(PDOException $e)
    {
   		 echo $e->getMessage();
    }
?>
